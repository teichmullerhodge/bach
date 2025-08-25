#include "songplayer.h"

void song_play(Song *song) {
  if (song->stream != NULL) {
    g_object_unref(song->stream);
  }

  song->stream = gtk_media_file_new_for_filename(song->path);

  if (song->stream == NULL) {
    g_error("Failed to create media stream for file: %s", song->path);
    return;
  }

  gtk_media_stream_play(song->stream);
  song->state = SONG_STATE_PLAYING;

  gtk_button_set_icon_name(GTK_BUTTON(song->playButton),
                           "media-playback-pause");
}
void song_pause(Song *song) {
  if (song->stream != NULL && GTK_IS_MEDIA_STREAM(song->stream)) {
    gtk_media_stream_pause(song->stream);
    song->state = SONG_STATE_PAUSED;

    // Atualiza ícone do botão para "play"
    gtk_button_set_icon_name(GTK_BUTTON(song->playButton),
                             "media-playback-start");
  }
}

void song_resume(Song *song) {
  if (song->stream != NULL && GTK_IS_MEDIA_STREAM(song->stream)) {
    gtk_media_stream_play(song->stream);
    song->state = SONG_STATE_PLAYING;

    gtk_button_set_icon_name(GTK_BUTTON(song->playButton),
                             "media-playback-pause");
  }
}

void song_stop(Song *song) {
  if (song->stream != NULL) {
    gtk_media_stream_pause(song->stream);
    g_object_unref(song->stream);
    song->stream = NULL;
  }
  song->state = SONG_STATE_IDLE;

  gtk_button_set_icon_name(GTK_BUTTON(song->playButton),
                           "media-playback-start");
}