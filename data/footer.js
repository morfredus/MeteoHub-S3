document.addEventListener("DOMContentLoaded", () => {
    const footer = document.querySelector('footer');
    if (footer) {
        footer.innerHTML = `
            MeteoHub S3 Web Interface - v<span id="version">--</span> 
            <a href="/files.html" class="footer-icon" title="Gestionnaire de fichiers">💾</a>
            <a href="/logs" class="footer-icon" title="Logs Système">📜</a>
        `;
        
        // Récupération de la version pour l'affichage
        fetch('/api/system')
            .then(response => response.json())
            .then(data => {
                const vSpan = document.getElementById('version');
                if (vSpan) vSpan.innerText = data.version;
            })
            .catch(err => console.error('Erreur version:', err));
    }
});