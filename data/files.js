async function fetchFiles() {
    try {
        const res = await fetch('/api/files/list');
        const files = await res.json();
        const tbody = document.getElementById('fileList');
        tbody.innerHTML = '';
        
        files.sort((a, b) => a.name.localeCompare(b.name));

        files.forEach(f => {
            const tr = document.createElement('tr');
            tr.innerHTML = `
                <td>${f.name}</td>
                <td>${(f.size / 1024).toFixed(2)} KB</td>
                <td>
                    <a href="/api/files/download?path=${f.name}" class="btn-icon" title="Télécharger">⬇️</a>
                    <button onclick="deleteFile('${f.name}')" class="btn-icon delete" title="Supprimer">🗑️</button>
                </td>
            `;
            tbody.appendChild(tr);
        });
        document.getElementById('status').textContent = "En ligne";
        document.getElementById('status').style.color = "#0f0";
    } catch (e) {
        console.error(e);
        document.getElementById('status').textContent = "Erreur";
    }
}

async function deleteFile(path) {
    if(!confirm(`Supprimer ${path} ?`)) return;
    try {
        await fetch(`/api/files/delete?path=${path}`, { method: 'DELETE' });
        fetchFiles();
    } catch(e) { alert('Erreur suppression'); }
}

async function uploadFile() {
    const input = document.getElementById('fileInput');
    if(input.files.length === 0) return;
    
    const file = input.files[0];
    const formData = new FormData();
    formData.append("file", file);

    document.getElementById('uploadStatus').textContent = "Envoi en cours...";
    
    try {
        const res = await fetch('/api/files/upload', {
            method: 'POST',
            body: formData
        });
        if(res.ok) {
            document.getElementById('uploadStatus').textContent = "Succès !";
            input.value = '';
            fetchFiles();
        } else {
            document.getElementById('uploadStatus').textContent = "Échec.";
        }
    } catch(e) {
        document.getElementById('uploadStatus').textContent = "Erreur réseau.";
    }
}

async function fetchSystem() {
    try {
        const res = await fetch('/api/system');
        const data = await res.json();
        document.getElementById('version').textContent = data.version;
    } catch (e) {}
}

window.onload = () => {
    fetchSystem();
    fetchFiles();
};