#include "appconfig/appconfig.h"
#include "definitions/definitions.h"
#include "glib-object.h"
#include "helpers/vecbox.h"
#include "metadata_r/metadata_r.h"
#include "songplayer/songplayer.h"
#include "state/appstate.h"
#include "state/shared_state.h"
#include "state/song_browser_state.h"
#include "state/song_state.h"
#include "state/touchable_state.h"
#include "widgets/widget_constructor.h"
#include "widgets/widget_label.h"
#include "widgets/widget_song_slider.h"
#include "widgets/widget_touchable.h"
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

  // GtkWidget *root = gtk_grid_new();
  // gtk_widget_add_css_class(root, "root-grid");
  // gtk_widget_set_hexpand(root, TRUE);
  // gtk_widget_set_vexpand(root, TRUE);

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
      "Browse songs", "audio-card-symbolic", "new-song-button",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});

  GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  VecBox *musics = box_new();
  if (musics == NULL)
    return;

  appState->window = window;
  appState->sidebar = sidebar;
  appState->musicsCards = musics;
  appState->selectedPath = NULL;
  appState->song = NULL;
  appState->songTitle = NULL;
  appState->songArtist = NULL;
  appState->lastSelectedCard = NULL;
  appState->songSlider = NULL;
  appState->minLabel = NULL;
  appState->maxLabel = NULL;

  g_signal_connect(newSong, "clicked", G_CALLBACK(on_new_song_clicked),
                   appState);

  gtk_widget_set_size_request(newSong, 80, -1);

  gtk_box_append(GTK_BOX(sidebarBox), musicsLabel);
  gtk_box_append(GTK_BOX(sidebarBox), entry);
  gtk_box_append(GTK_BOX(sidebarBox), newSong);

  GtkWidget *scroll = gtk_scrolled_window_new();

  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), sidebar);

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
  gtk_box_append(GTK_BOX(sidebar), nothingFound);

  // main area

  // GtkWidget *mainArea = gtk_grid_new();
  // gtk_widget_add_css_class(mainArea, "main-grid");
  // gtk_widget_set_hexpand(mainArea, TRUE);
  // gtk_widget_set_vexpand(mainArea, TRUE);

  GtkWidget *mainArea =
      widget_construct(gtk_grid_new, "main-area",
                       &(WidgetPositioning){TRUE, TRUE, GTK_ALIGN_BASELINE_FILL,
                                            GTK_ALIGN_BASELINE_FILL});

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
  appState->minLabel = minLabel;
  appState->maxLabel = maxLabel;

  gtk_widget_set_size_request(sliderContainer, 500, -1);

  GtkGesture *gesture = gtk_gesture_click_new();
  g_signal_connect(gesture, "released", G_CALLBACK(on_slider_release), NULL);
  gtk_widget_add_controller(songSlider, GTK_EVENT_CONTROLLER(gesture));

  //   g_signal_connect(songSlider, "change-value",
  //   G_CALLBACK(on_slider_change),
  //                    appState);

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
  g_timeout_add(1000, update_song_slider, appState);
  // g_signal_connect(ctrl, "enter", G_CALLBACK(on_card_entered), container);

  GtkWidget *controlsContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  gtk_widget_add_css_class(controlsContainer, "controls-container");
  gtk_widget_set_halign(controlsContainer, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(controlsContainer), favoriteMusic);

  gtk_box_append(GTK_BOX(controlsContainer), backMusic);
  gtk_box_append(GTK_BOX(controlsContainer), playMusic);
  gtk_box_append(GTK_BOX(controlsContainer), forwardMusic);

  gtk_box_append(GTK_BOX(controlsContainer), shuffleMusic);

  gtk_grid_attach(GTK_GRID(mainArea), musicImage, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(mainArea), sliderContainer, 0, 1, 1, 1);

  gtk_grid_attach(GTK_GRID(mainArea), songTitle, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(mainArea), songArtist, 0, 3, 1, 1);

  gtk_grid_attach(GTK_GRID(mainArea), controlsContainer, 0, 4, 1, 1);

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
