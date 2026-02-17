let chart;

async function fetchLive() {
    try {
        const res = await fetch('/api/live');
        const data = await res.json();
        document.getElementById('temp').textContent = data.temp.toFixed(1);
        document.getElementById('hum').textContent = data.hum.toFixed(0);
        document.getElementById('pres').textContent = data.pres.toFixed(1);
        document.getElementById('status').textContent = "En ligne";
        document.getElementById('status').style.color = "#0f0";
    } catch (e) {
        document.getElementById('status').textContent = "Déconnecté";
        document.getElementById('status').style.color = "#f00";
    }
}

async function fetchHistory() {
    try {
        const res = await fetch('/api/history');
        const json = await res.json();
        updateChart(json.data);
    } catch (e) { console.error("Erreur historique", e); }
}

async function fetchSystem() {
    try {
        const res = await fetch('/api/system');
        const data = await res.json();
        document.getElementById('version').textContent = data.version;
    } catch (e) {}
}

async function fetchStats() {
    try {
        const res = await fetch('/api/stats');
        const data = await res.json();
        const tbody = document.getElementById('statsBody');
        if(!tbody) return;
        
        tbody.innerHTML = `
            <tr>
                <td>Température (°C)</td>
                <td>${data.temp.min.toFixed(1)}</td>
                <td>${data.temp.avg.toFixed(1)}</td>
                <td>${data.temp.max.toFixed(1)}</td>
            </tr>
            <tr>
                <td>Humidité (%)</td>
                <td>${data.hum.min.toFixed(0)}</td>
                <td>${data.hum.avg.toFixed(0)}</td>
                <td>${data.hum.max.toFixed(0)}</td>
            </tr>
            <tr>
                <td>Pression (hPa)</td>
                <td>${data.pres.min.toFixed(0)}</td>
                <td>${data.pres.avg.toFixed(0)}</td>
                <td>${data.pres.max.toFixed(0)}</td>
            </tr>
        `;
        document.getElementById('status').textContent = "En ligne";
        document.getElementById('status').style.color = "#0f0";
    } catch (e) {
        console.error("Erreur stats", e);
    }
}

function updateChart(data) {
    if (!chart) return;
    chart.data.labels = data.map(d => new Date(d.t * 1000).toLocaleTimeString());
    chart.data.datasets[0].data = data.map(d => d.temp);
    chart.data.datasets[1].data = data.map(d => d.hum);
    chart.data.datasets[2].data = data.map(d => d.pres);
    chart.update();
}

function initChart() {
    const ctx = document.getElementById('historyChart').getContext('2d');
    chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Température (°C)',
                data: [],
                borderColor: '#00a8ff',
                backgroundColor: 'rgba(0, 168, 255, 0.1)',
                tension: 0.4,
                yAxisID: 'y'
            },
            {
                label: 'Humidité (%)',
                data: [],
                borderColor: '#00ff88',
                backgroundColor: 'rgba(0, 255, 136, 0.1)',
                tension: 0.4,
                yAxisID: 'y1',
                hidden: false
            },
            {
                label: 'Pression (hPa)',
                data: [],
                borderColor: '#ff00ff',
                backgroundColor: 'rgba(255, 0, 255, 0.1)',
                tension: 0.4,
                yAxisID: 'y2',
                hidden: false
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            interaction: { mode: 'index', intersect: false },
            scales: {
                y: {
                    type: 'linear',
                    display: true,
                    position: 'left',
                    title: { display: true, text: 'Temp (°C)', color: '#00a8ff' },
                    ticks: { color: '#00a8ff' }
                },
                y1: {
                    type: 'linear',
                    display: true,
                    position: 'right',
                    grid: { drawOnChartArea: false },
                    title: { display: true, text: 'Hum (%)', color: '#00ff88' },
                    ticks: { color: '#00ff88' }
                },
                y2: {
                    type: 'linear',
                    display: true,
                    position: 'right',
                    grid: { drawOnChartArea: false },
                    title: { display: true, text: 'Pres (hPa)', color: '#ff00ff' },
                    ticks: { color: '#ff00ff' }
                }
            }
        }
    });
}

window.onload = () => {
    initChart();
    fetchSystem();
    fetchLive();
    fetchHistory();
    setInterval(fetchLive, 5000);
};