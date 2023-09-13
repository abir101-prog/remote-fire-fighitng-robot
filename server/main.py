from http.server import HTTPServer, BaseHTTPRequestHandler

class esp32http(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.send_header('content-type', 'application/json')
        self.end_headers()

        self.