/* =========================================
   PUBLISHER RANKING SYSTEM
   Calculate and display publisher rankings
========================================= */

function calculatePublisherStats() {
  const games = JSON.parse(localStorage.getItem("games")) || [];
  const publisherStats = {};

  // Calculate stats for each publisher
  games.forEach(game => {
    if (!game.publisher) return;

    if (!publisherStats[game.publisher]) {
      publisherStats[game.publisher] = {
        studio: game.studioName || game.publisher,
        publisher: game.publisher,
        totalSales: 0,
        totalRevenue: 0,
        gameCount: 0,
        games: []
      };
    }

    const sales = game.salesCount || 0;
    const revenue = sales * game.price;

    publisherStats[game.publisher].totalSales += sales;
    publisherStats[game.publisher].totalRevenue += revenue;
    publisherStats[game.publisher].gameCount += 1;
    publisherStats[game.publisher].games.push(game.name);
  });

  // Convert to array and sort by total sales
  const rankings = Object.values(publisherStats).sort((a, b) => b.totalSales - a.totalSales);

  // Add rank numbers
  rankings.forEach((publisher, index) => {
    publisher.rank = index + 1;
  });

  return rankings;
}

function getPublisherRank(publisherUsername) {
  const rankings = calculatePublisherStats();
  const publisher = rankings.find(p => p.publisher === publisherUsername);
  return publisher || null;
}

function displayTopPublishers(limit = 10) {
  const rankings = calculatePublisherStats();
  const topPublishers = rankings.slice(0, limit);

  console.log("=== TOP PUBLISHERS ===");
  topPublishers.forEach(p => {
    console.log(`#${p.rank} ${p.studio} - ${p.totalSales} sales - Rp ${p.totalRevenue.toLocaleString('id-ID')}`);
  });

  return topPublishers;
}

function renderPublisherLeaderboard(containerId, limit = 10) {
  const container = document.getElementById(containerId);
  if (!container) return;

  const rankings = calculatePublisherStats();
  const topPublishers = rankings.slice(0, limit);

  if (topPublishers.length === 0) {
    container.innerHTML = `<p style="text-align:center; color:#999;">Belum ada publisher dengan penjualan.</p>`;
    return;
  }

  let html = `
    <div style="background: var(--bg-card); border-radius: 10px; padding: 20px;">
      <h2 style="margin-top: 0; color: var(--primary);">🏆 Top Publishers</h2>
      <table style="width: 100%; border-collapse: collapse;">
        <thead>
          <tr style="border-bottom: 2px solid var(--primary);">
            <th style="padding: 10px; text-align: left;">Rank</th>
            <th style="padding: 10px; text-align: left;">Studio</th>
            <th style="padding: 10px; text-align: center;">Games</th>
            <th style="padding: 10px; text-align: right;">Sales</th>
            <th style="padding: 10px; text-align: right;">Revenue</th>
          </tr>
        </thead>
        <tbody>
  `;

  topPublishers.forEach((p, index) => {
    const medalEmoji = index === 0 ? '🥇' : index === 1 ? '🥈' : index === 2 ? '🥉' : '';
    html += `
      <tr style="border-bottom: 1px solid #3d3f5e;">
        <td style="padding: 12px; font-weight: bold;">${medalEmoji} #${p.rank}</td>
        <td style="padding: 12px;">${p.studio}</td>
        <td style="padding: 12px; text-align: center;">${p.gameCount}</td>
        <td style="padding: 12px; text-align: right; color: var(--primary);">${p.totalSales}</td>
        <td style="padding: 12px; text-align: right; color: #2ecc71; font-weight: bold;">
          Rp ${p.totalRevenue.toLocaleString('id-ID')}
        </td>
      </tr>
    `;
  });

  html += `
        </tbody>
      </table>
    </div>
  `;

  container.innerHTML = html;
}

function renderPublisherOwnRank(publisherUsername) {
  const rankData = getPublisherRank(publisherUsername);
  
  if (!rankData) {
    return `
      <div style="background: var(--bg-card); border-radius: 10px; padding: 20px; margin-bottom: 20px;">
        <h3>📊 Your Publisher Stats</h3>
        <p style="color: #999;">Belum ada game yang terjual. Mulai submit game untuk meningkatkan ranking!</p>
      </div>
    `;
  }

  const rankColor = rankData.rank <= 3 ? '#ffd700' : rankData.rank <= 10 ? '#4ea8de' : '#999';
  const rankEmoji = rankData.rank === 1 ? '🥇' : rankData.rank === 2 ? '🥈' : rankData.rank === 3 ? '🥉' : '📊';

  return `
    <div style="background: linear-gradient(135deg, rgba(78,168,222,0.1), rgba(43,45,66,0.9)); 
                border-radius: 10px; padding: 25px; margin-bottom: 20px; 
                border: 2px solid ${rankColor};">
      <h3 style="margin-top: 0; color: ${rankColor};">${rankEmoji} Your Publisher Ranking</h3>
      <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 20px; margin-top: 20px;">
        <div style="text-align: center;">
          <div style="font-size: 32px; font-weight: bold; color: ${rankColor};">#{rankData.rank}</div>
          <div style="color: #999; font-size: 14px;">Global Rank</div>
        </div>
        <div style="text-align: center;">
          <div style="font-size: 32px; font-weight: bold; color: var(--primary);">${rankData.totalSales}</div>
          <div style="color: #999; font-size: 14px;">Total Sales</div>
        </div>
        <div style="text-align: center;">
          <div style="font-size: 24px; font-weight: bold; color: #2ecc71;">
            Rp ${(rankData.totalRevenue / 1000).toFixed(0)}K
          </div>
          <div style="color: #999; font-size: 14px;">Total Revenue</div>
        </div>
        <div style="text-align: center;">
          <div style="font-size: 32px; font-weight: bold; color: #f1c40f;">${rankData.gameCount}</div>
          <div style="color: #999; font-size: 14px;">Published Games</div>
        </div>
      </div>
    </div>
  `;
}

// Update game sales count when purchased
function incrementGameSales(gameId) {
  let games = JSON.parse(localStorage.getItem("games")) || [];
  const gameIndex = games.findIndex(g => g.id === gameId);
  
  if (gameIndex !== -1) {
    games[gameIndex].salesCount = (games[gameIndex].salesCount || 0) + 1;
    localStorage.setItem("games", JSON.stringify(games));
  }
}
