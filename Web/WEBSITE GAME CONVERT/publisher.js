// ===== IMAGE HANDLING =====
let imageBase64 = null; // Menyimpan gambar dalam format Base64

// Preview Gambar sebelum Upload
function previewImage() {
  const fileInput = document.getElementById("gameImage");
  const preview = document.getElementById("imagePreview");
  const previewImg = document.getElementById("previewImg");

  if (fileInput.files && fileInput.files[0]) {
    const file = fileInput.files[0];

    // Validasi ukuran file (max 2MB)
    if (file.size > 2 * 1024 * 1024) {
      alert("Ukuran file terlalu besar! Maksimal 2MB.");
      fileInput.value = "";
      return;
    }

    // Validasi tipe file
    if (!file.type.match('image.*')) {
      alert("File harus berupa gambar (JPG/PNG)!");
      fileInput.value = "";
      return;
    }

    const reader = new FileReader();

    reader.onload = function (e) {
      imageBase64 = e.target.result; // Simpan Base64
      previewImg.src = imageBase64;
      preview.style.display = "block";
    };

    reader.readAsDataURL(file);
  }
}

// Hapus Gambar
function clearImage() {
  document.getElementById("gameImage").value = "";
  document.getElementById("imagePreview").style.display = "none";
  imageBase64 = null;
}

// Submit Game (UPDATE)
function submitGame() {
  const name = document.getElementById("gameName").value.trim();
  const price = parseInt(document.getElementById("gamePrice").value);
  const category = document.getElementById("gameCategory").value.trim();

  if (!name || !price || !category) {
    alert("Nama, Harga, dan Kategori wajib diisi!");
    return;
  }

  if (price < 0) {
    alert("Harga tidak boleh negatif!");
    return;
  }

  // Gunakan imageBase64 jika ada, atau null untuk default
  if (submitGameForApproval(name, price, category, imageBase64)) {
    // Reset form
    document.getElementById("gameName").value = "";
    document.getElementById("gamePrice").value = "";
    document.getElementById("gameCategory").value = "";
    clearImage();

    renderPendingGames();
  }
}

// Check Publisher Access
function checkPublisherAccess() {
  const user = getCurrentUser();
  if (!user || user.role !== "publisher") {
    alert("Akses ditolak! Hanya Publisher yang bisa mengakses halaman ini.");
    window.location.href = "index.html";
    return false;
  }
  return true;
}

if (!checkPublisherAccess()) {
  throw new Error("Unauthorized access");
}

// Display Studio Info
const user = getCurrentUser();
document.getElementById("studioInfo").innerText =
  `Studio: ${user.studioName} | ${user.country} | Since ${user.yearFounded}`;


// Render Pending Games
// Render Pending Games (UPDATE - dengan preview gambar)
function renderPendingGames() {
  const pending = getPendingGames();
  const div = document.getElementById("pendingList");

  if (pending.length === 0) {
    div.innerHTML = "<p>Tidak ada game yang menunggu verifikasi.</p>";
    return;
  }

  div.innerHTML = "";
  pending.forEach((game, index) => {
    const card = document.createElement("div");
    card.className = "card";
    card.style.display = "flex";
    card.style.gap = "15px";
    card.style.alignItems = "center";

    card.innerHTML = `
      <img src="${game.image}" alt="${game.name}" 
           style="width:100px; height:70px; object-fit:cover; border-radius:4px; flex-shrink:0;">
      
      <div style="flex:1;">
        <strong>${game.name}</strong><br>
        Rp ${game.price.toLocaleString('id-ID')} | ${game.category}<br>
        <span class="status-pending">⏳ Menunggu Admin (Posisi Antrian: ${index + 1})</span>
      </div>
    `;
    div.appendChild(card);
  });
}

// Render Approved Games
// Render Approved Games (UPDATE - dengan gambar)
function renderApprovedGames() {
  const approved = getApprovedGames();
  const tbody = document.getElementById("approvedList");

  if (approved.length === 0) {
    tbody.innerHTML = "<tr><td colspan='6' style='text-align:center'>Belum ada game yang disetujui</td></tr>";
    return;
  }

  tbody.innerHTML = "";
  approved.forEach(game => {
    const tr = document.createElement("tr");
    tr.innerHTML = `
      <td>
        <img src="${game.image}" alt="${game.name}" 
             style="width:60px; height:40px; object-fit:cover; border-radius:4px; vertical-align:middle; margin-right:10px;">
        ${game.name}
      </td>
      <td>Rp ${game.price.toLocaleString('id-ID')}</td>
      <td>${game.category}</td>
      <td>${game.salesCount || 0}</td>
      <td>${game.rating || 0}</td>
      <td><button onclick="editGame(${game.id})">Edit</button></td>
    `;
    tbody.appendChild(tr);
  });
}

// ===== REJECTION NOTIFICATIONS =====
function checkRejectionNotifications() {
  const user = getCurrentUser();
  if (!user) return;

  let rejections = JSON.parse(localStorage.getItem("rejections")) || [];
  const userRejections = rejections.filter(r => r.publisher === user.username && r.status === 'unread');

  if (userRejections.length > 0) {
    displayRejectionBanner(userRejections);
  }
}

function displayRejectionBanner(rejections) {
  // Create notification banner at top of page
  const banner = document.createElement("div");
  banner.id = "rejectionBanner";
  banner.style.cssText = `
    background: linear-gradient(135deg, #e63946, #d62828);
    color: white;
    padding: 20px;
    margin: -20px -20px 20px -20px;
    border-radius: 0 0 10px 10px;
    box-shadow: 0 4px 10px rgba(230, 57, 70, 0.3);
  `;

  let html = `
    <div style="display: flex; align-items: center; gap: 15px; margin-bottom: 15px;">
      <div style="flex: 1;">
        <h3 style="margin: 0 0 5px 0; font-size: 18px;">Game Ditolak Admin</h3>
        <p style="margin: 0; opacity: 0.9;">Anda memiliki ${rejections.length} game yang ditolak</p>
      </div>
      <button onclick="clearAllRejections()" style="
        background: rgba(255,255,255,0.2);
        border: 1px solid white;
        color: white;
        padding: 8px 16px;
        border-radius: 5px;
        cursor: pointer;
        font-weight: bold;
      ">Tandai Semua Dibaca</button>
    </div>
  `;

  rejections.forEach((rejection, index) => {
    const date = new Date(rejection.timestamp);
    const dateStr = date.toLocaleDateString('id-ID');
    const timeStr = date.toLocaleTimeString('id-ID');

    html += `
      <div style="
        background: rgba(255,255,255,0.1);
        padding: 15px;
        border-radius: 8px;
        margin-bottom: ${index < rejections.length - 1 ? '10px' : '0'};
        border-left: 4px solid white;
      ">
        <div style="display: flex; justify-content: space-between; align-items: start; margin-bottom: 8px;">
          <strong style="font-size: 16px;">${rejection.gameName}</strong>
          <span style="font-size: 12px; opacity: 0.8;">${dateStr} ${timeStr}</span>
        </div>
        <div style="opacity: 0.9; margin-bottom: 8px;">
          <strong>Alasan:</strong> ${rejection.reason}
        </div>
        <button onclick="clearRejection('${rejection.timestamp}')" style="
          background: rgba(255,255,255,0.2);
          border: 1px solid rgba(255,255,255,0.5);
          color: white;
          padding: 5px 12px;
          border-radius: 4px;
          cursor: pointer;
          font-size: 12px;
        ">Tandai Dibaca</button>
      </div>
    `;
  });

  banner.innerHTML = html;

  // Insert at top of wrapper
  const wrapper = document.querySelector('.wrapper') || document.body;
  wrapper.insertBefore(banner, wrapper.firstChild);
}

function clearRejection(timestamp) {
  let rejections = JSON.parse(localStorage.getItem("rejections")) || [];
  const index = rejections.findIndex(r => r.timestamp === timestamp);

  if (index !== -1) {
    rejections[index].status = 'read';
    localStorage.setItem("rejections", JSON.stringify(rejections));

    // Remove banner and re-check
    const banner = document.getElementById("rejectionBanner");
    if (banner) banner.remove();
    checkRejectionNotifications();
  }
}

function clearAllRejections() {
  const user = getCurrentUser();
  if (!user) return;

  let rejections = JSON.parse(localStorage.getItem("rejections")) || [];
  rejections.forEach(r => {
    if (r.publisher === user.username && r.status === 'unread') {
      r.status = 'read';
    }
  });
  localStorage.setItem("rejections", JSON.stringify(rejections));

  // Remove banner
  const banner = document.getElementById("rejectionBanner");
  if (banner) banner.remove();

  alert("Semua notifikasi telah ditandai sebagai dibaca!");
}

// Edit Game (Simplified)
function editGame(gameId) {
  const newName = prompt("Nama baru:");
  const newPrice = parseInt(prompt("Harga baru:"));
  const newCategory = prompt("Kategori baru:");

  if (newName && newPrice && newCategory) {
    updatePublisherGame(gameId, newName, newPrice, newCategory);
    renderApprovedGames();
  }
}

// Init
renderPendingGames();
renderApprovedGames();
checkRejectionNotifications(); // Check for rejection notifications on page load
