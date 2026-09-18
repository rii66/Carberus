#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <WebSocketsServer.h>

#include "webserver.h"
#include "web.h"

// Global instances (extern di webserver.h)
WebServer server(80);
WebSocketsServer webSocket(81);

void initWebServer() {

  server.on("/", []() {
    server.send_P(200, "text/html", index_html);
  });

  server.on(
    "/update",
    HTTP_POST,

    []() {
      server.send(
        200,
        "text/plain",
        Update.hasError() ? "FAIL" : "OK"
      );

      if (!Update.hasError()) {
        delay(500);
        ESP.restart();
      }
    },

    []() {
      HTTPUpload& upload = server.upload();

      if (upload.status == UPLOAD_FILE_START) {
        Update.begin(UPDATE_SIZE_UNKNOWN);
      }
      else if (upload.status == UPLOAD_FILE_WRITE) {
        Update.write(upload.buf, upload.currentSize);
      }
      else if (upload.status == UPLOAD_FILE_END) {
        Update.end(true);
      }
    }
  );

  server.begin();
}
