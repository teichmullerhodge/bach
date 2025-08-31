#include "appconfig/appconfig.h"
#include "definitions/definitions.h"
#include "glib-object.h"
#include "helpers/dirreader.h"
#include "helpers/vecbox.h"
#include "metadata_r/metadata_r.h"
#include "songplayer/songplayer.h"
#include "state/appstate.h"
#include "state/shared_state.h"
#include "state/song_browser_state.h"
#include "state/song_slider_state.h"
#include "state/song_state.h"
#include "state/touchable_state.h"
#include "view/view.h"
#include "widgets/widget_constructor.h"
#include "widgets/widget_dialog.h"
#include "widgets/widget_label.h"
#include "widgets/widget_song_slider.h"
#include "widgets/widget_touchable.h"
#include "widgets/widget_volume_controller.h"
#include "widgets/widgets.h"
#include "widgets/widgets_config.h"
#include <gtk/gtk.h>
#include <string.h>
#include <time.h>

static void on_activate(GtkApplication *app) {

  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "./resources/css/index.css");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_USER);

  g_object_unref(provider);

  AppState *appState = malloc(sizeof(AppState));
  if (appState == NULL)
    return;

  Song *song = song_new("./resources/song/beautiful.mp3"); // default song
  if (song == NULL) {
    printf("Error allocating memory for song structure.");
    return;
  }

  GtkWidget *window = widget_window(
      app, APP_CONFIG_WIN_TITLE, APP_CONFIG_WIN_WIDTH, APP_CONFIG_WIN_HEIGHT);

  gtk_widget_add_css_class(window, "window-main");

  GtkWidget *root =
      widget_construct(gtk_grid_new, "root-application",
                       &(WidgetPositioning){TRUE, TRUE, GTK_ALIGN_BASELINE_FILL,
                                            GTK_ALIGN_BASELINE_FILL});

  // sidebar header
  GtkWidget *sidebarBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  GtkWidget *musicsLabel = widget_label(
      "Musics", "sidebar-title",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  GtkWidget *entry = gtk_search_entry_new();
  gtk_widget_set_name(entry, "MusicSearch");

  GtkWidget *newSong = touchable(
      "+", "folder-music-symbolic", "header-button",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});
  GtkWidget *musicView = touchable(
      "Playing", "audio-card-symbolic", "header-button",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  GtkWidget *playlistView = touchable(
      "Playlist", "view-list-symbolic", "header-button",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  GtkWidget *sidebar =
      gtk_grid_new(); // gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  GtkWidget *playlistGrid = gtk_grid_new();
  GtkWidget *p1 = playlist_card(
      "playlist-card",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});
  GtkWidget *p2 = playlist_card(
      "playlist-card",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});
  GtkWidget *p3 = playlist_card(
      "playlist-card",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  gtk_grid_attach(GTK_GRID(playlistGrid), p1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(playlistGrid), p2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(playlistGrid), p3, 0, 2, 1, 1);

  appState->playlistGrid = playlistGrid;

  VecBox *musics = box_new();
  if (musics == NULL)
    return;

  appState->window = window;
  appState->musicsCards = musics;
  appState->selectedPath = NULL;
  appState->song = NULL;
  appState->songTitle = NULL;
  appState->songArtist = NULL;
  appState->lastSelectedCard = NULL;
  appState->songSlider = NULL;
  appState->minLabel = NULL;
  appState->maxLabel = NULL;

  appState->songsGrid = sidebar;
  appState->rowCount = 0;
  g_signal_connect(newSong, "clicked", G_CALLBACK(on_new_song_clicked),
                   appState);
  g_signal_connect(playlistView, "clicked", G_CALLBACK(playlist_view),
                   appState);

  g_signal_connect(musicView, "clicked", G_CALLBACK(music_view), appState);

  gtk_widget_set_size_request(newSong, 40, -1);
  gtk_widget_set_size_request(musicView, 40, -1);
  gtk_widget_set_size_request(playlistView, 40, -1);

  GtkWidget *buttonsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_widget_set_vexpand(buttonsBox, FALSE);
  gtk_widget_set_hexpand(buttonsBox, FALSE);

  gtk_box_append(GTK_BOX(buttonsBox), newSong);
  gtk_box_append(GTK_BOX(buttonsBox), musicView);

  gtk_box_append(GTK_BOX(buttonsBox), playlistView);

  gtk_box_append(GTK_BOX(sidebarBox), musicsLabel);
  gtk_box_append(GTK_BOX(sidebarBox), entry);
  gtk_box_append(GTK_BOX(sidebarBox), buttonsBox);

  GtkWidget *scroll = gtk_scrolled_window_new();
  appState->sideScroll = scroll;

  appState->viewStack = gtk_stack_new();
  gtk_stack_add_named(GTK_STACK(appState->viewStack), appState->songsGrid,
                      "songs");
  gtk_stack_add_named(GTK_STACK(appState->viewStack), appState->playlistGrid,
                      "playlist");
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(appState->sideScroll),
                                appState->viewStack);

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll),
                                appState->viewStack);

  gtk_widget_set_vexpand(scroll, FALSE);
  gtk_widget_set_hexpand(scroll, FALSE);

  gtk_widget_set_valign(scroll, GTK_ALIGN_START);
  gtk_widget_set_halign(scroll, GTK_ALIGN_START);

  gtk_widget_set_size_request(scroll, 250, APP_CONFIG_WIN_HEIGHT);

  GtkWidget *nothingFound = not_found();

  gtk_widget_set_vexpand(nothingFound, TRUE);
  gtk_widget_set_hexpand(nothingFound, TRUE);
  gtk_widget_set_halign(nothingFound, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(nothingFound, GTK_ALIGN_CENTER);

  gtk_widget_set_name(nothingFound, "NotFound");
  gtk_widget_set_visible(nothingFound, FALSE);

  g_signal_connect(entry, "changed", G_CALLBACK(on_search_changed), sidebar);

  gtk_box_append(GTK_BOX(sidebarBox), scroll);

  gtk_grid_attach(GTK_GRID(appState->songsGrid), nothingFound, 0,
                  appState->rowCount, 1, 1);
  appState->rowCount++;

  GtkWidget *mainArea =
      widget_construct(gtk_grid_new, "main-area",
                       &(WidgetPositioning){TRUE, TRUE, GTK_ALIGN_BASELINE_FILL,
                                            GTK_ALIGN_BASELINE_FILL});
  appState->mainArea = mainArea;

  gtk_grid_set_row_homogeneous(GTK_GRID(mainArea), FALSE);

  GtkWidget *musicImage = widget_image(
      "./resources/start-layout.png", 375, "music-image",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});

  /// returns a GtkWidget* box, the song slider is the first child.
  GtkWidget *sliderContainer = widget_song_slider(
      0.0f, 20.0f, STEP_MUSIC_DEFAULT, "music-slider",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});

  GtkWidget *songSlider = gtk_widget_get_first_child(sliderContainer);
  GtkWidget *labelsRow = gtk_widget_get_last_child(sliderContainer);
  GtkWidget *minLabel = gtk_widget_get_first_child(labelsRow);
  GtkWidget *maxLabel = gtk_widget_get_last_child(labelsRow);

  appState->songSlider = songSlider;

  // Crie o controlador legacy para capturar eventos raw
  GtkEventController *legacy_controller = gtk_event_controller_legacy_new();
  gtk_event_controller_set_propagation_phase(legacy_controller,
                                             GTK_PHASE_CAPTURE);
  gtk_widget_add_controller(songSlider, legacy_controller);
  g_signal_connect(legacy_controller, "event", G_CALLBACK(on_slider_released),
                   appState);
  g_signal_connect(songSlider, "value-changed", G_CALLBACK(on_slider_change),
                   appState);
  g_timeout_add(100, (GSourceFunc)update_slider_cb, appState);
  appState->minLabel = minLabel;
  appState->maxLabel = maxLabel;

  appState->minLabel = minLabel;
  appState->maxLabel = maxLabel;

  gtk_widget_set_size_request(sliderContainer, 500, -1);

  gtk_widget_add_css_class(songSlider, "music-scale");

  GtkWidget *songTitle = widget_label(
      "", "song-title",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});
  GtkWidget *songArtist = widget_label(
      "No song selected", "song-artist",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START}

  );

  appState->songTitle = songTitle;
  appState->songArtist = songArtist;

  GtkWidget *favoriteMusic =
      gtk_button_new_from_icon_name("non-starred-symbolic");
  gtk_widget_add_css_class(favoriteMusic, "media-control-button-minor");
  gtk_widget_set_name(favoriteMusic, "favoriteButton");

  GtkWidget *backMusic = gtk_button_new_from_icon_name("media-skip-backward");
  gtk_widget_add_css_class(backMusic, "media-control-button");
  gtk_widget_set_name(backMusic, "backwardButton");

  GtkWidget *playMusic = gtk_button_new_from_icon_name("media-playback-start");
  gtk_widget_add_css_class(playMusic, "media-control-button");

  song->playButton = playMusic;
  appState->song = song;

  GtkWidget *forwardMusic = gtk_button_new_from_icon_name("media-skip-forward");
  gtk_widget_add_css_class(forwardMusic, "media-control-button");
  gtk_widget_set_name(forwardMusic, "forwardButton");

  GtkWidget *shuffleMusic =
      gtk_button_new_from_icon_name("media-playlist-shuffle-symbolic");
  gtk_widget_add_css_class(shuffleMusic, "media-control-button-minor");
  gtk_widget_set_name(shuffleMusic, "shuffleButton");

  g_signal_connect(playMusic, "clicked", G_CALLBACK(song_state), appState);

  GtkWidget *controlsContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  GtkWidget *volumeControl = volume_controller(
      GTK_ORIENTATION_HORIZONTAL, "volume-controller",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_CENTER});
  GtkWidget *volumeSlider = gtk_widget_get_last_child(volumeControl);
  g_signal_connect(volumeSlider, "value-changed", G_CALLBACK(on_volume_change),
                   appState);

  gtk_widget_set_size_request(volumeControl, 100, -1);

  gtk_widget_add_css_class(controlsContainer, "controls-container");
  gtk_widget_set_halign(controlsContainer, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(controlsContainer, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(controlsContainer), favoriteMusic);

  gtk_box_append(GTK_BOX(controlsContainer), backMusic);
  gtk_box_append(GTK_BOX(controlsContainer), playMusic);
  gtk_box_append(GTK_BOX(controlsContainer), forwardMusic);

  gtk_box_append(GTK_BOX(controlsContainer), shuffleMusic);

  gtk_box_append(GTK_BOX(controlsContainer), volumeControl);

  gtk_grid_attach(GTK_GRID(mainArea), musicImage, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(mainArea), sliderContainer, 0, 1, 1, 1);

  gtk_grid_attach(GTK_GRID(mainArea), songTitle, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(mainArea), songArtist, 0, 3, 1, 1);

  gtk_grid_attach(GTK_GRID(mainArea), controlsContainer, 0, 4, 1, 1);

  gtk_widget_set_visible(sliderContainer, FALSE);
  gtk_widget_set_visible(controlsContainer, FALSE);

  bool musicLoaded = load_music_cards(appState);
  if (musicLoaded == false) {
    printf("No music files found in the Music directory.\n");
  }
  appState->sliderContainer = sliderContainer;
  appState->controlsContainer = controlsContainer;

  gtk_grid_attach(GTK_GRID(root), sidebarBox, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(root), mainArea, 1, 0, 1, 1);

  gtk_window_set_child(GTK_WINDOW(window), root);

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {

  GtkApplication *app =
      gtk_application_new(APP_CONFIG_APP_ID, APP_CONFIG_APP_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
