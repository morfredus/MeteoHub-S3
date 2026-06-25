// Affichage de l'alerte météo sur le dashboard
let current_alert_payload = null;

// Injection dynamique des valeurs min/max via graph_config.js
// (graph_config.js est généré lors du build à partir de config.h)
let GRAPH_SCALE_MODE = window.GRAPH_CONFIG?.scale_mode ?? 2;
let GRAPH_SCALE_MARGIN_PCT = window.GRAPH_CONFIG?.scale_margin_pct ?? 20;
const GRAPH_TEMP_MIN = window.GRAPH_CONFIG?.temp_min ?? -10.0;
const GRAPH_TEMP_MAX = window.GRAPH_CONFIG?.temp_max ?? 40.0;
const GRAPH_HUM_MIN  = window.GRAPH_CONFIG?.hum_min  ?? 20.0;
const GRAPH_HUM_MAX  = window.GRAPH_CONFIG?.hum_max  ?? 90.0;
const GRAPH_PRES_MIN = window.GRAPH_CONFIG?.pres_min ?? 970.0;
const GRAPH_PRES_MAX = window.GRAPH_CONFIG?.pres_max ?? 1040.0;

// Ajout UI : contrôle du mode d'échelle et du pourcentage
function setGraphScaleMode(mode) {
    GRAPH_SCALE_MODE = mode;
    updateChartScale();
}
function setGraphScaleMarginPct(pct) {
    GRAPH_SCALE_MARGIN_PCT = pct;
    document.getElementById('scaleMarginValue').textContent = pct;
    updateChartScale();
}

function getDynamicMinMax(data, key, userMin, userMax) {
    let dynMin = Math.min(...data.map(d => d[key]));
    let dynMax = Math.max(...data.map(d => d[key]));
    if (dynMin === dynMax) {
        dynMin -= 0.3;
        dynMax += 0.3;
    }
    if (GRAPH_SCALE_MODE === 1) {
        return [dynMin, dynMax];
    } else if (GRAPH_SCALE_MODE === 2) {
        let range = dynMax - dynMin;
        let margin = range * (GRAPH_SCALE_MARGIN_PCT / 100);
        let min = dynMin - margin;
        let max = dynMax + margin;
        min = Math.max(min, userMin);
        max = Math.min(max, userMax);
        return [min, max];
    } else {
        return [userMin, userMax];
    }
}

function updateChartScale() {
    if (!chart || !chart.data || !chart.data.datasets) return;
    const data = chart.data.labels.length > 0 ? chart.data.datasets[0].data.map((_, i) => ({
        temp: chart.data.datasets[0].data[i],
        hum: chart.data.datasets[1].data[i],
        pres: chart.data.datasets[2].data[i]
    })) : [];
    if (data.length === 0) return;
    let [tmin, tmax] = getDynamicMinMax(data, 'temp', GRAPH_TEMP_MIN, GRAPH_TEMP_MAX);
    let [hmin, hmax] = getDynamicMinMax(data, 'hum', GRAPH_HUM_MIN, GRAPH_HUM_MAX);
    let [pmin, pmax] = getDynamicMinMax(data, 'pres', GRAPH_PRES_MIN, GRAPH_PRES_MAX);
    chart.options.scales.y.min = tmin;
    chart.options.scales.y.max = tmax;
    chart.options.scales.y1.min = hmin;
    chart.options.scales.y1.max = hmax;
    chart.options.scales.y2.min = pmin;
    chart.options.scales.y2.max = pmax;
    chart.update();
}

function getAlertThemeClass(level) {
    if (level >= 3) return 'alert-level-red';
    if (level === 2) return 'alert-level-orange';
    if (level === 1) return 'alert-level-yellow';
    return 'alert-level-none';
}

function formatAlertValidity(startUnix, endUnix) {
    if (!Number.isFinite(startUnix) || !Number.isFinite(endUnix) || startUnix <= 0 || endUnix <= 0) {
        return 'Validité : non précisée';
    }

    const startText = new Date(startUnix * 1000).toLocaleString('fr-FR');
    const endText = new Date(endUnix * 1000).toLocaleString('fr-FR');
    return `Validité : du ${startText} au ${endText}`;
}

function applyAlertCardTheme(level) {
    const alertCard = document.getElementById('alertCard');
    if (!alertCard) return;

    alertCard.classList.remove('alert-level-none', 'alert-level-yellow', 'alert-level-orange', 'alert-level-red');
    alertCard.classList.add(getAlertThemeClass(level));
}

function renderSensorValidityBadge(isValid) {
    const badge = document.getElementById('sensorInvalidBadge');
    if (!badge) return;

    if (isValid === false) {
        badge.hidden = false;
    } else {
        badge.hidden = true;
    }
}

function escapeHtmlAndBreakLines(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML.replace(/\n/g, '<br>');
}

function renderAlertCard(data) {
    const alertText = document.getElementById('alertText');
    const alertValidity = document.getElementById('alertValidity');
    const detailsBody = document.getElementById('alertDetailsBody');
    if (!alertText || !alertValidity) return;

    if (data.alert_active || data.active) {
        const level = Number.isFinite(data.alert_severity) ? data.alert_severity : (Number.isFinite(data.severity) ? data.severity : 0);
        const levelLabel = data.alert_level_label_fr || 'Alerte';
        const event = data.alert_event_fr || data.event_fr || data.alert_event || data.event || 'Alerte météo';
        const senderValue = data.alert_sender || data.sender || 'Inconnu';
        const summary = data.description_fr || data.alert_description_fr || '';
        const rawDescription = data.description_raw || data.alert_description_raw || '';
        const startUnix = data.alert_start_unix || data.start_unix;
        const endUnix = data.alert_end_unix || data.end_unix;

        alertText.textContent = `${levelLabel} (${level}) - ${event}`;
        alertText.style.fontWeight = '700';
        alertValidity.textContent = formatAlertValidity(startUnix, endUnix);
        applyAlertCardTheme(level);

        if (detailsBody) {
            const sourceBlock = rawDescription
                ? `<p><strong>Bulletin source (${escapeHtmlAndBreakLines(senderValue)}, langue d'origine) :</strong></p><p>${escapeHtmlAndBreakLines(rawDescription)}</p>`
                : '';
            detailsBody.innerHTML = `<p>${escapeHtmlAndBreakLines(summary)}</p>${sourceBlock}<p><strong>Consigne :</strong> Surveillez l’évolution locale et limitez les déplacements non essentiels.</p>`;
        }
    } else {
        alertText.textContent = 'Aucune alerte météo en cours.';
        alertText.style.fontWeight = '500';
        alertValidity.textContent = 'Validité : --';
        applyAlertCardTheme(0);
        if (detailsBody) detailsBody.innerHTML = '';
    }
}

async function fetchAlert() {
    const alertText = document.getElementById('alertText');
    if (!alertText) return;

    try {
        const res = await fetch('/api/alert');
        const data = await res.json();
        current_alert_payload = data;
        renderAlertCard(data);
    } catch (e) {
        alertText.textContent = "Erreur lors de la récupération de l'alerte météo.";
        applyAlertCardTheme(0);
    }
}
let chart;

const HISTORY_WINDOWS_SECONDS = {
    short: 2 * 60 * 60,
    long: 24 * 60 * 60
};

const HISTORY_REFRESH_MS = 15000;
const LIVE_REFRESH_MS = 5000;
const ALERT_REFRESH_MS = 15 * 60 * 1000;
const STATS_REFRESH_MS = 15000;
const FORECAST7_REFRESH_MS = 30 * 60 * 1000;

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

        renderSensorValidityBadge(data.sensor_valid);

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

        const trendBody = document.getElementById('trendBody');
        const trendGlobal = document.getElementById('trendGlobal');
        if (trendBody) {
            const trend = data.trend || {};
            const t = trend.temp || {};
            const h = trend.hum || {};
            const p = trend.pres || {};
            const available48h = !!trend.available_48h;
            const globalLabel = trend.global_label_fr || 'Tendance stable';

            // Affiche la variation et la direction ensemble (ex: "+0.8 °C ↗")
            const arrow = (direction) => {
                if (direction === 'hausse') return '↗';
                if (direction === 'baisse') return '↘';
                if (direction === 'indisponible') return '';
                return '→';
            };
            const cell = (delta, direction, unit, decimals) => {
                if (direction === 'indisponible') return 'N/D';
                const sign = delta > 0 ? '+' : '';
                return `${sign}${(delta ?? 0).toFixed(decimals)} ${unit} ${arrow(direction)}`;
            };

            trendBody.innerHTML = `
                <tr>
                    <td>Température</td>
                    <td>${cell(t.delta_1h, t.direction_1h, '°C', 1)}</td>
                    <td>${cell(t.delta_12h, t.direction_12h, '°C', 1)}</td>
                    <td>${cell(t.delta_24h, t.direction_24h, '°C', 1)}</td>
                    <td>${available48h ? cell(t.delta_48h, t.direction_48h, '°C', 1) : 'N/D'}</td>
                </tr>
                <tr>
                    <td>Humidité</td>
                    <td>${cell(h.delta_1h, h.direction_1h, '%', 1)}</td>
                    <td>${cell(h.delta_12h, h.direction_12h, '%', 1)}</td>
                    <td>${cell(h.delta_24h, h.direction_24h, '%', 1)}</td>
                    <td>${available48h ? cell(h.delta_48h, h.direction_48h, '%', 1) : 'N/D'}</td>
                </tr>
                <tr>
                    <td>Pression</td>
                    <td>${cell(p.delta_1h, p.direction_1h, 'hPa', 1)}</td>
                    <td>${cell(p.delta_12h, p.direction_12h, 'hPa', 1)}</td>
                    <td>${cell(p.delta_24h, p.direction_24h, 'hPa', 1)}</td>
                    <td>${available48h ? cell(p.delta_48h, p.direction_48h, 'hPa', 1) : 'N/D'}</td>
                </tr>
            `;

            if (trendGlobal) {
                trendGlobal.innerHTML = `<strong>Tendance générale (1h/12h/24h${available48h ? '/48h' : ''}) :</strong> ${globalLabel}`;
            }
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

function windDirectionFr(deg) {
    const directions = ['nord', 'nord-est', 'est', 'sud-est', 'sud', 'sud-ouest', 'ouest', 'nord-ouest'];
    const index = Math.round((deg % 360) / 45) % 8;
    return directions[index];
}

function formatForecastDayLabel(dtUnix, isFirst) {
    if (isFirst) return 'Demain';
    const weekday = new Date(dtUnix * 1000).toLocaleDateString('fr-FR', { weekday: 'long' });
    return weekday.charAt(0).toUpperCase() + weekday.slice(1);
}

// Génère un court résumé en français pour un jour de prévision, à partir des
// données brutes OpenWeatherMap (température, probabilité/volume de pluie,
// vent). Heuristique simple, pas une traduction exacte d'un bulletin officiel.
function buildForecastDayNarrative(day, prevDay, isFirst) {
    const tempMax = day.temp_max;
    const pop = day.pop || 0;
    const rain = day.rain_mm || 0;
    const windKmh = (day.wind_speed || 0) * 3.6;
    const popPct = Math.round(pop * 100);

    let heat;
    if (tempMax >= 35) heat = 'caniculaire';
    else if (tempMax >= 30) heat = 'très chaud';
    else if (tempMax >= 25) heat = 'chaud';
    else if (tempMax >= 18) heat = 'doux';
    else heat = 'frais';

    let trendPhrase;
    if (prevDay) {
        const delta = tempMax - prevDay.temp_max;
        if (delta <= -4) {
            trendPhrase = `Le break météo. Les températures chutent nettement avec un max à ${tempMax.toFixed(1)}°C`;
        } else if (delta <= -1.5) {
            trendPhrase = `Légère baisse mais ${heat}. Max à ${tempMax.toFixed(1)}°C`;
        } else if (delta >= 4) {
            trendPhrase = `Nette hausse, ${heat}. Max à ${tempMax.toFixed(1)}°C`;
        } else if (delta >= 1.5) {
            trendPhrase = `Légère hausse, ${heat}. Max à ${tempMax.toFixed(1)}°C`;
        } else {
            trendPhrase = `Stable, ${heat}. Max à ${tempMax.toFixed(1)}°C`;
        }
    } else {
        trendPhrase = `Toujours ${heat}. Max à ${tempMax.toFixed(1)}°C`;
    }

    let rainPhrase;
    if (pop >= 0.8 || rain >= 5) {
        rainPhrase = `C'est la journée la plus pluvieuse de la série : ${popPct}% de chances de précipitations${rain > 0 ? ` avec ${rain.toFixed(1)} mm de pluie` : ''}.`;
    } else if (pop >= 0.4) {
        rainPhrase = `Le risque de pluie augmente (${popPct}%).`;
    } else if (pop >= 0.15) {
        rainPhrase = `Légère pluie fine prévue (probabilité ${popPct}%)${rain > 0 ? `, ${rain.toFixed(1)} mm attendus` : ''}.`;
    } else {
        rainPhrase = 'Beau ciel dégagé, aucune pluie.';
    }

    let windPhrase = '';
    if (windKmh >= 15) {
        windPhrase = ` Le vent souffle du ${windDirectionFr(day.wind_deg || 0)} autour de ${Math.round(windKmh)} km/h.`;
    }

    return `${trendPhrase}. ${rainPhrase}${windPhrase}`;
}

async function fetchForecast7() {
    const container = document.getElementById('forecast7Body');
    if (!container) return;

    try {
        const res = await fetch('/api/forecast7');
        const days = await res.json();

        if (!Array.isArray(days) || days.length === 0) {
            container.textContent = 'Prévisions indisponibles.';
            return;
        }

        let prev = null;
        const parts = days.map((day, index) => {
            const label = formatForecastDayLabel(day.dt, index === 0);
            const narrative = buildForecastDayNarrative(day, prev, index === 0);
            prev = day;
            return `<div class="forecast-day">
                <div class="forecast-day-title">${escapeHtmlAndBreakLines(label)}</div>
                <div class="forecast-day-text">${escapeHtmlAndBreakLines(narrative)}</div>
            </div>`;
        });

        container.innerHTML = parts.join('');
    } catch (e) {
        container.textContent = 'Erreur lors de la récupération des prévisions.';
    }
}

function updateChart(data) {
    if (!chart) return;
    chart.data.labels = data.map((d) => new Date(d.t * 1000).toLocaleTimeString());
    chart.data.datasets[0].data = data.map((d) => d.temp);
    chart.data.datasets[1].data = data.map((d) => d.hum);
    chart.data.datasets[2].data = data.map((d) => d.pres);
    updateChartScale();
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
    updateChartScale();
}

window.onload = () => {
    fetchSystem();
    fetchLive();
    fetchAlert();

    if (isHistoryPage()) {
        initChart();
        fetchHistory();
        setInterval(fetchHistory, HISTORY_REFRESH_MS);
        // Synchronise le slider avec la valeur initiale
        const marginSlider = document.getElementById('scaleMargin');
        const marginValue = document.getElementById('scaleMarginValue');
        if (marginSlider && marginValue) {
            marginSlider.value = GRAPH_SCALE_MARGIN_PCT;
            marginValue.textContent = GRAPH_SCALE_MARGIN_PCT;
        }
        // Synchronise le select avec la valeur initiale
        const modeSelect = document.getElementById('scaleMode');
        if (modeSelect) modeSelect.value = GRAPH_SCALE_MODE;
    }

    if (isStatsPage()) {
        fetchStats();
        setInterval(fetchStats, STATS_REFRESH_MS);
        fetchForecast7();
        setInterval(fetchForecast7, FORECAST7_REFRESH_MS);
    }

    setInterval(fetchLive, LIVE_REFRESH_MS);
    setInterval(fetchAlert, ALERT_REFRESH_MS);
};
