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
#include "widgets/widget_label.h"
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

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), APP_CONFIG_WIN_TITLE);
  gtk_window_set_default_size(GTK_WINDOW(window), APP_CONFIG_WIN_WIDTH,
                              APP_CONFIG_WIN_HEIGHT);

  GtkWidget *root = gtk_grid_new();
  gtk_widget_add_css_class(root, "root-grid");
  gtk_widget_set_hexpand(root, TRUE);
  gtk_widget_set_vexpand(root, TRUE);

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

  GtkWidget *sidebarGrid = gtk_grid_new();
  VecBox *musics = box_new();
  if (musics == NULL)
    return;

  AppState *appState = malloc(sizeof(AppState));
  if (appState == NULL)
    return;
  appState->rowCount = 1; // starts at 1 because of the not found widget
  appState->window = window;
  appState->sidebarGrid = sidebarGrid;
  appState->musicsCards = musics;
  appState->selectedPath = NULL;
  appState->song = NULL;
  g_signal_connect(newSong, "clicked", G_CALLBACK(on_new_song_clicked),
                   appState);

  gtk_widget_set_size_request(newSong, 80, -1);

  gtk_box_append(GTK_BOX(sidebarBox), musicsLabel);
  gtk_box_append(GTK_BOX(sidebarBox), entry);
  gtk_box_append(GTK_BOX(sidebarBox), newSong);

  GtkWidget *scroll = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), sidebarGrid);

  gtk_widget_add_css_class(root, "sidebar-grid");

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

  g_signal_connect(entry, "changed", G_CALLBACK(on_search_changed),
                   sidebarGrid);

  gtk_grid_attach(GTK_GRID(sidebarGrid), nothingFound, 0, 0, 1, 1);

  // for (int i = 0; i < 30; i++) {
  //   GtkWidget *card = music_card(
  //       "./resources/GTK.png", "GTK Song", "Teichmuller",
  //       &(WidgetPositioning){TRUE, TRUE, GTK_ALIGN_CENTER,
  //       GTK_ALIGN_CENTER});
  //   gtk_widget_set_size_request(card, 245, -1);
  //   gtk_grid_attach(GTK_GRID(sidebarGrid), card, 0, i + 1, 1, 1);
  // }

  gtk_box_append(GTK_BOX(sidebarBox), scroll);

  // main area

  GtkWidget *mainArea = gtk_grid_new();
  gtk_widget_add_css_class(mainArea, "main-grid");
  gtk_widget_set_hexpand(mainArea, TRUE);
  gtk_widget_set_vexpand(mainArea, TRUE);

  gtk_grid_set_row_homogeneous(GTK_GRID(mainArea), FALSE);

  GtkWidget *musicImage = widget_image(
      "./resources/GTK.png", 100, "music-image",
      &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});

  GtkWidget *songTitle = widget_label(
      "Song Title", "song-title",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});
  GtkWidget *songArtist = widget_label(
      "Song Artist", "song-artist",
      &(WidgetPositioning){FALSE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START}

  );

  GtkWidget *backMusic = gtk_button_new_from_icon_name("media-skip-backward");
  gtk_widget_add_css_class(backMusic, "media-control-button");
  gtk_widget_set_name(backMusic, "backwardButton");

  GtkWidget *playMusic = gtk_button_new_from_icon_name("media-playback-start");
  gtk_widget_add_css_class(playMusic, "media-control-button");

  GtkWidget *forwardMusic = gtk_button_new_from_icon_name("media-skip-forward");
  gtk_widget_add_css_class(forwardMusic, "media-control-button");
  gtk_widget_set_name(forwardMusic, "forwardButton");

  Song *song = malloc(sizeof(Song));
  if (song == NULL) {
    printf("Error allocating memory for song structure.");
    return;
  }

  song->path = "./resources/song/beautiful.mp3"; // default song
  song->stream = NULL;
  song->playButton = playMusic;
  song->state = SONG_STATE_IDLE;

  appState->song = song;

  g_signal_connect(playMusic, "clicked", G_CALLBACK(song_state),
                   appState->song);

  // g_signal_connect(ctrl, "enter", G_CALLBACK(on_card_entered), container);

  GtkWidget *controlsContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_widget_add_css_class(controlsContainer, "controls-container");
  gtk_widget_set_halign(controlsContainer, GTK_ALIGN_CENTER);

  gtk_box_append(GTK_BOX(controlsContainer), backMusic);
  gtk_box_append(GTK_BOX(controlsContainer), playMusic);
  gtk_box_append(GTK_BOX(controlsContainer), forwardMusic);

  gtk_grid_attach(GTK_GRID(mainArea), musicImage, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(mainArea), songTitle, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(mainArea), songArtist, 0, 2, 1, 1);

  gtk_grid_attach(GTK_GRID(mainArea), controlsContainer, 0, 3, 1, 1);

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
