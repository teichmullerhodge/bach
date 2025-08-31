#include "dirreader.h"
#include "../metadata_r/metadata_r.h"
#include "../state/appstate.h"
#include "../widgets/widget_music_card.h"
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static const char *get_home_dir(void) {
  const char *home = getenv("HOME");
  if (home)
    return home;

  struct passwd *pw = getpwuid(getuid());
  return pw ? pw->pw_dir : ".";
}

static void get_music_dir(char *buffer, size_t size) {
  FILE *fp = popen("xdg-user-dir MUSIC 2>/dev/null", "r");
  if (fp && fgets(buffer, size, fp)) {
    buffer[strcspn(buffer, "\n")] = 0;
    pclose(fp);
    if (buffer[0] != '\0')
      return;
  }

  const char *home = get_home_dir();
  snprintf(buffer, size, "%s/Music", home);
}

bool load_music_cards(AppState *appState) {
  printf("Loading music cards...\n");
  char musicDir[1024];
  get_music_dir(musicDir, sizeof(musicDir));

  DIR *dir = opendir(musicDir);
  if (!dir) {
    perror("opendir");
    return false;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    if (g_str_has_suffix(entry->d_name, ".mp3") == FALSE &&
        g_str_has_suffix(entry->d_name, ".wav") == FALSE &&
        g_str_has_suffix(entry->d_name, ".flac") == FALSE) {
      printf("Skipping non-audio file: %s\n", entry->d_name);
      continue;
    }

    char fullpath[2048];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", musicDir, entry->d_name);
    MetadataR *mref = collect_metadata(fullpath);
    if (mref == NULL || mref->path == NULL || mref->title == NULL ||
        mref->artist == NULL) {
      printf("Failed to read metadata for file: %s\n", fullpath);
      continue;
    }

    GtkWidget *song = music_card(
        "./resources/GTK.png", mref->title, mref->artist, mref->seconds,
        &(WidgetPositioning){TRUE, FALSE, GTK_ALIGN_CENTER, GTK_ALIGN_START});

    gtk_grid_attach(GTK_GRID(appState->songsGrid), song, 0, appState->rowCount,
                    1, 1);
    appState->rowCount++;

    gtk_widget_set_name(
        song,
        mref->path); // set the path as the name, will be used later for ref.

    GtkGesture *click = gtk_gesture_click_new();
    gtk_widget_add_controller(song, GTK_EVENT_CONTROLLER(click));

    g_signal_connect(click, "pressed", G_CALLBACK(select_song), appState);
  }

  closedir(dir);
  return true;
}
