const grid = document.getElementById("libraryGrid");
const games = JSON.parse(localStorage.getItem("games")) || [];
const currentUserUsername = localStorage.getItem("currentUser");

function renderLibrary() {
    if (!currentUserUsername) {
        grid.innerHTML = "<p>Silakan login untuk melihat library.</p>";
        return;
    }

    const users = JSON.parse(localStorage.getItem("users")) || [];
    const user = users.find(u => u.username === currentUserUsername);

    if (!user || !user.library || user.library.length === 0) {
        grid.innerHTML = "<div style='text-align:center; width:100%; padding:50px;'><h3>Library Anda Kosong</h3><p>Silakan beli game di Store.</p></div>";
        return;
    }

    grid.innerHTML = "";

    user.library.forEach(gameId => {
        const game = games.find(g => g.id === gameId);
        if (!game) return;

        const isDownloaded = user.downloadedGames && user.downloadedGames.includes(gameId);
        const card = document.createElement("div");
        card.className = "card";

        function escapeHTML(text) {
            const div = document.createElement('div');
            div.textContent = text;
            return div.innerHTML;
        }

        const downloadButton = isDownloaded
            ? `<button onclick="deleteDownload(${gameId})" 
                  style="width:100%; background:#e63946; border:none; color:white; padding:8px; border-radius:4px; cursor:pointer; font-weight:bold;">
                  HAPUS DOWNLOAD
               </button>`
            : `<button onclick="downloadGameToDevice(${gameId})" 
                  style="width:100%; background:#2ecc71; border:none; color:white; padding:8px; border-radius:4px; cursor:pointer; font-weight:bold;">
                  DOWNLOAD
               </button>`;

        const downloadStatus = isDownloaded
            ? `<div style="background:#2ecc71; color:white; padding:4px 8px; border-radius:4px; font-size:11px; margin-bottom:8px; text-align:center;">DOWNLOADED</div>`
            : '';

        card.innerHTML = `
            <img src="${escapeHTML(game.image || 'gmbar/default.jpg')}" alt="${escapeHTML(game.name)}" 
                 style="width:100%; height:150px; object-fit:cover; border-radius:4px; display:block; margin-bottom:10px;">
            <h4 style="margin:10px 0 5px 0">${escapeHTML(game.name)}</h4>
            <div style="font-size:12px; color:#aaa; margin-bottom:10px;">${escapeHTML(game.category)}</div>
            ${downloadStatus}
            ${downloadButton}
        `;
        grid.appendChild(card);
    });
}

// Download game to device
function downloadGameToDevice(gameId) {
    if (typeof addToDownloaded === "function") {
        addToDownloaded(gameId);
        renderLibrary();
    } else {
        alert("Fungsi download belum tersedia!");
    }
}

// Delete downloaded game
function deleteDownload(gameId) {
    if (confirm("Hapus game dari download list?")) {
        if (typeof removeFromDownloaded === "function") {
            removeFromDownloaded(gameId);
            renderLibrary();
        }
    }
}

document.addEventListener("DOMContentLoaded", renderLibrary);