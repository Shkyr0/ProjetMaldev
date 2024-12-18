import http.server
import socketserver
import os  # Import pour obtenir le répertoire courant

# Définir le port
PORT = 1234

# Créer un gestionnaire de requêtes HTTP
Handler = http.server.SimpleHTTPRequestHandler

# Configurer et démarrer le serveur
with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print(f"Serveur démarré sur le port {PORT}")
    print(f"Répertoire de travail actuel : {os.getcwd()}")  # Affiche le PWD
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nArrêt du serveur.")
        httpd.server_close()
