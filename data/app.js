// Affichage de l'alerte météo sur le dashboard
async function fetchAlert() {
    const alertCard = document.getElementById('alert-card');
    const alertContent = document.getElementById('alert-content');
    if (!alertCard || !alertContent) return;
    try {
        const res = await fetch('/api/alert');
        const data = await res.json();
        alertCard.classList.remove('alert-yellow', 'alert-orange', 'alert-red');
        if (data.active) {
            let colorClass = '';
            let label = '';
            if (data.severity >= 3) { colorClass = 'alert-red'; label = 'Alerte rouge'; }
            else if (data.severity === 2) { colorClass = 'alert-orange'; label = 'Alerte orange'; }
            else { colorClass = 'alert-yellow'; label = 'Alerte jaune'; }
            alertCard.classList.add(colorClass);
            alertContent.innerHTML = `${label} : <b>${data.event}</b> <span style="font-weight:normal">(${data.sender})</span>`;
        } else {
            alertContent.textContent = "Aucune alerte météo en cours.";
        }
    } catch (e) {
        alertContent.textContent = "Erreur lors de la récupération de l'alerte météo.";
        alertCard.classList.remove('alert-yellow', 'alert-orange', 'alert-red');
    }
}
let chart;

const HISTORY_WINDOWS_SECONDS = {
    short: 2 * 60 * 60,
    long: 24 * 60 * 60
};

const HISTORY_REFRESH_MS = 15000;
const LIVE_REFRESH_MS = 5000;
const STATS_REFRESH_MS = 15000;

function getPageName() {
    return document.body?.dataset?.page || 'dashboard';
}

function isHistoryPage() {
    const page = getPageName();
    return page === 'dashboard' || page === 'longterm';
}

function isStatsPage() {
    return getPageName() === 'stats';
}

function getHistoryWindowSeconds() {
    return getPageName() === 'longterm' ? HISTORY_WINDOWS_SECONDS.long : HISTORY_WINDOWS_SECONDS.short;
}

function getHistoryIntervalSeconds() {
    return getPageName() === 'longterm' ? 30 * 60 : 5 * 60;
}

function buildHistoryUrl() {
    const params = new URLSearchParams({
        window: String(getHistoryWindowSeconds()),
        interval: String(getHistoryIntervalSeconds())
    });
    return `/api/history?${params.toString()}`;
}

async function fetchLive() {
    try {
        const res = await fetch('/api/live');
        const data = await res.json();

        const temp = document.getElementById('temp');
        const hum = document.getElementById('hum');
        const pres = document.getElementById('pres');
        if (temp) temp.textContent = data.temp.toFixed(1);
        if (hum) hum.textContent = data.hum.toFixed(0);
        if (pres) pres.textContent = data.pres.toFixed(1);

        const status = document.getElementById('status');
        if (status) {
            status.textContent = 'En ligne';
            status.style.color = '#0f0';
        }
    } catch (e) {
        const status = document.getElementById('status');
        if (status) {
            status.textContent = 'Déconnecté';
            status.style.color = '#f00';
        }
    }
}

async function fetchHistory() {
    if (!chart || !isHistoryPage()) return;

    try {
        const res = await fetch(buildHistoryUrl());
        const json = await res.json();
        updateChart(Array.isArray(json.data) ? json.data : []);
    } catch (e) {
        console.error('Erreur historique', e);
    }
}

async function fetchSystem() {
    try {
        const res = await fetch('/api/system');
        const data = await res.json();
        const version = document.getElementById('version');
        if (version) version.textContent = data.project_version || data.version || '--';
    } catch (e) {}
}

async function fetchStats() {
    if (!isStatsPage()) return;

    try {
        const res = await fetch('/api/stats');
        const data = await res.json();
        const tbody = document.getElementById('statsBody');
        if (!tbody) return;

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

        // Ajout de la tendance météo
        const trendBody = document.getElementById('trendBody');
        if (trendBody && data.trend) {
            trendBody.innerHTML = `
                <tr>
                    <td>Température</td>
                    <td>${data.trend.temp.delta_1h.toFixed(1)}°C (${data.trend.temp.direction_1h})</td>
                    <td>${data.trend.temp.delta_24h.toFixed(1)}°C (${data.trend.temp.direction_24h})</td>
                    <td>${data.trend.temp.direction_1h === data.trend.temp.direction_24h ? data.trend.temp.direction_1h : data.trend.temp.direction_1h + '/' + data.trend.temp.direction_24h}</td>
                </tr>
                <tr>
                    <td>Humidité</td>
                    <td>${data.trend.hum.delta_1h.toFixed(0)}% (${data.trend.hum.direction_1h})</td>
                    <td>${data.trend.hum.delta_24h.toFixed(0)}% (${data.trend.hum.direction_24h})</td>
                    <td>${data.trend.hum.direction_1h === data.trend.hum.direction_24h ? data.trend.hum.direction_1h : data.trend.hum.direction_1h + '/' + data.trend.hum.direction_24h}</td>
                </tr>
                <tr>
                    <td>Pression</td>
                    <td>${data.trend.pres.delta_1h.toFixed(1)} hPa (${data.trend.pres.direction_1h})</td>
                    <td>${data.trend.pres.delta_24h.toFixed(1)} hPa (${data.trend.pres.direction_24h})</td>
                    <td>${data.trend.pres.direction_1h === data.trend.pres.direction_24h ? data.trend.pres.direction_1h : data.trend.pres.direction_1h + '/' + data.trend.pres.direction_24h}</td>
                </tr>
            `;
        }

        const status = document.getElementById('status');
        if (status) {
            status.textContent = 'En ligne';
            status.style.color = '#0f0';
        }
    } catch (e) {
        console.error('Erreur stats', e);
    }
}

function updateChart(data) {
    if (!chart) return;
    chart.data.labels = data.map((d) => new Date(d.t * 1000).toLocaleTimeString());
    chart.data.datasets[0].data = data.map((d) => d.temp);
    chart.data.datasets[1].data = data.map((d) => d.hum);
    chart.data.datasets[2].data = data.map((d) => d.pres);
    chart.update();
}

function initChart() {
    const chart_canvas = document.getElementById('historyChart');
    if (!chart_canvas || !isHistoryPage()) return;

    const ctx = chart_canvas.getContext('2d');
    chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [
                {
                    label: 'Température (°C)',
                    data: [],
                    borderColor: '#00a8ff',
                    backgroundColor: 'rgba(0, 168, 255, 0.1)',
                    tension: 0.45,
                    cubicInterpolationMode: 'monotone',
                    pointRadius: 0,
                    stepped: false,
                    yAxisID: 'y'
                },
                {
                    label: 'Humidité (%)',
                    data: [],
                    borderColor: '#00ff88',
                    backgroundColor: 'rgba(0, 255, 136, 0.1)',
                    tension: 0.45,
                    cubicInterpolationMode: 'monotone',
                    pointRadius: 0,
                    stepped: false,
                    yAxisID: 'y1',
                    hidden: false
                },
                {
                    label: 'Pression (hPa)',
                    data: [],
                    borderColor: '#ff00ff',
                    backgroundColor: 'rgba(255, 0, 255, 0.1)',
                    tension: 0.45,
                    cubicInterpolationMode: 'monotone',
                    pointRadius: 0,
                    stepped: false,
                    yAxisID: 'y2',
                    hidden: false
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            animation: false,
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
    if (getPageName() === 'dashboard') {
        fetchAlert();
        setInterval(fetchAlert, 15000);
    }
    fetchSystem();
    fetchLive();

    if (isHistoryPage()) {
        initChart();
        fetchHistory();
        setInterval(fetchHistory, HISTORY_REFRESH_MS);
    }

    if (isStatsPage()) {
        fetchStats();
        setInterval(fetchStats, STATS_REFRESH_MS);
    }

    setInterval(fetchLive, LIVE_REFRESH_MS);
};
