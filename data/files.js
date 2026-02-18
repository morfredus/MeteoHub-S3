let currentFs = 'littlefs';
let currentPath = '/';

document.addEventListener('DOMContentLoaded', () => {
    const urlParams = new URLSearchParams(window.location.search);
    if (urlParams.has('fs')) {
        currentFs = urlParams.get('fs');
    }
    
    updateInterface();
    loadFiles(currentPath);
});

function switchFs(fsName) {
    // Rechargement simple pour changer de contexte
    window.location.href = `/files.html?fs=${fsName}`;
}

function updateInterface() {
    document.getElementById('fsTitle').innerText = (currentFs === 'sd') ? 'Carte SD' : 'Mémoire Interne (LittleFS)';
    
    const btnLittle = document.getElementById('btn-littlefs');
    const btnSd = document.getElementById('btn-sd');
    
    // Style visuel pour le bouton actif
    if(currentFs === 'sd') { btnSd.style.opacity = '1'; btnLittle.style.opacity = '0.5'; }
    else { btnLittle.style.opacity = '1'; btnSd.style.opacity = '0.5'; }
}

function loadFiles(path) {
    currentPath = path;
    document.getElementById('currentPath').innerText = currentPath;
    
    const tbody = document.getElementById('fileList');
    tbody.innerHTML = '<tr><td colspan="3">Chargement...</td></tr>';

    fetch(`/api/files/list?fs=${currentFs}&path=${encodeURIComponent(path)}`)
        .then(response => {
            if (!response.ok) throw new Error("Erreur ou SD absente");
            return response.json();
        })
        .then(files => {
            tbody.innerHTML = '';
            if (files.length === 0) {
                tbody.innerHTML = '<tr><td colspan="3">Dossier vide</td></tr>';
                return;
            }
            
            // Trier: Dossiers d'abord, puis fichiers
            files.sort((a, b) => {
                if (a.isDir && !b.isDir) return -1;
                if (!a.isDir && b.isDir) return 1;
                return a.name.localeCompare(b.name);
            });

            files.forEach(file => {
                const tr = document.createElement('tr');
                const isDir = file.isDir;
                const name = file.name.startsWith('/') ? file.name.substring(1) : file.name; // Affichage propre
                const fullPath = (currentPath === '/' ? '' : currentPath) + '/' + name;

                let icon = isDir ? '📁' : '📄';
                let size = isDir ? '-' : formatSize(file.size);
                
                let actions = '';
                if (isDir) {
                    actions = `<button onclick="loadFiles('${fullPath}')" class="btn-small">Ouvrir</button>`;
                } else {
                    actions = `
                        <a href="/api/files/download?fs=${currentFs}&path=${encodeURIComponent(fullPath)}" class="btn-small" target="_blank">⬇️</a>
                        <button onclick="deleteFile('${fullPath}')" class="btn-small btn-danger">🗑️</button>
                    `;
                }

                tr.innerHTML = `
                    <td><span class="icon">${icon}</span> ${name}</td>
                    <td>${size}</td>
                    <td>${actions}</td>
                `;
                tbody.appendChild(tr);
            });
        })
        .catch(err => {
            tbody.innerHTML = `<tr><td colspan="3" style="color:red">Erreur: ${err.message}</td></tr>`;
        });
}

function navigateUp() {
    if (currentPath === '/') return;
    const parts = currentPath.split('/');
    parts.pop(); // Retirer dernier segment
    let newPath = parts.join('/');
    if (newPath === '') newPath = '/';
    loadFiles(newPath);
}

function deleteFile(path) {
    if(!confirm(`Supprimer ${path} ?`)) return;
    
    fetch(`/api/files/delete?fs=${currentFs}&path=${encodeURIComponent(path)}`, { method: 'DELETE' })
        .then(response => {
            if(response.ok) loadFiles(currentPath);
            else alert('Erreur suppression');
        });
}

function uploadFile() {
    const input = document.getElementById('fileInput');
    if(input.files.length === 0) return;
    
    const file = input.files[0];
    const formData = new FormData();
    // On upload dans le dossier courant
    let filename = (currentPath === '/' ? '' : currentPath) + '/' + file.name;
    formData.append("file", file, filename);
    
    const status = document.getElementById('uploadStatus');
    status.innerText = "Envoi en cours...";
    
    fetch(`/api/files/upload?fs=${currentFs}`, {
        method: 'POST',
        body: formData
    }).then(response => {
        if(response.ok) {
            status.innerText = "Succès !";
            loadFiles(currentPath);
        } else status.innerText = "Erreur upload";
    });
}

function formatSize(bytes) {
    if(bytes < 1024) return bytes + " B";
    if(bytes < 1048576) return (bytes/1024).toFixed(1) + " KB";
    return (bytes/1048576).toFixed(1) + " MB";
}