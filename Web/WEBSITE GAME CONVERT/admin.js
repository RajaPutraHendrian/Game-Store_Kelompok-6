
function checkAdminAccess() {
  const currentUser = localStorage.getItem("currentUser");
  const users = JSON.parse(localStorage.getItem("users")) || [];
  const user = users.find(u => u.username === currentUser);

  if (!user || user.role !== "admin") {
    alert("Akses ditolak! Hanya Admin yang bisa mengakses halaman ini.");
    window.location.href = "index.html";
    return false;
  }
  return true;
}

// Jalankan pengecekan saat halaman dimuat
if (!checkAdminAccess()) {
  document.body.innerHTML = "<h2 style='text-align:center; margin-top:100px;'>Unauthorized Access</h2>";
  throw new Error("Unauthorized access attempt");
}


/* =============================
   PURCHASE QUEUE & TRANSACTION STACK
============================= */

// ===== INIT STORAGE =====
if (!localStorage.getItem("purchaseQueue")) {
  localStorage.setItem("purchaseQueue", JSON.stringify([]));
}

if (!localStorage.getItem("transactionStack")) {
  localStorage.setItem("transactionStack", JSON.stringify([]));
}

let queue = JSON.parse(localStorage.getItem("purchaseQueue"));
let stack = JSON.parse(localStorage.getItem("transactionStack"));

// ===== ELEMENT =====
const queueDiv = document.getElementById("queueList");
const stackDiv = document.getElementById("stackList");

// ===== RENDER PURCHASE QUEUE =====
function renderQueue() {
  queueDiv.innerHTML = "";

  if (queue.length === 0) {
    queueDiv.innerHTML = "<p>No queue</p>";
    return;
  }

  queue.forEach((q, i) => {
    const d = document.createElement("div");
    d.className = "card";
    d.innerHTML = `${i + 1}. ${q.username} membeli Game ID ${q.gameId}`;
    queueDiv.appendChild(d);
  });
}

function renderStack() {
  stackDiv.innerHTML = "";

  if (stack.length === 0) {
    stackDiv.innerHTML = "<p>No transactions</p>";
    return;
  }

  // LIFO
  for (let i = stack.length - 1; i >= 0; i--) {
    const t = stack[i];
    const d = document.createElement("div");
    d.className = "card";
    d.innerHTML = `${t.username} | Game ID ${t.gameId}`;
    stackDiv.appendChild(d);
  }
}

// ===== PROCESS PURCHASE QUEUE =====
function processQueue() {
  if (queue.length === 0) {
    alert("Queue kosong");
    return;
  }

  // DEQUEUE (FIFO)
  const trx = queue.shift();

  // PUSH STACK
  stack.push(trx);

  localStorage.setItem("purchaseQueue", JSON.stringify(queue));
  localStorage.setItem("transactionStack", JSON.stringify(stack));

  alert("1 transaksi diproses");
  renderQueue();
  renderStack();
}

/* =============================
   PUBLISHER GAME VERIFICATION
============================= */

// ===== INIT PUBLISHER QUEUE =====
if (!localStorage.getItem("publisherQueue")) {
  localStorage.setItem("publisherQueue", JSON.stringify([]));
}

// ===== RENDER PUBLISHER QUEUE =====
function renderPublisherQueue() {
  const div = document.getElementById("publisherQueueList");
  if (!div) return;

  let publisherQueue = JSON.parse(localStorage.getItem("publisherQueue")) || [];

  if (publisherQueue.length === 0) {
    div.innerHTML = "<p>Tidak ada game menunggu verifikasi</p>";
    return;
  }

  div.innerHTML = "";
  publisherQueue.forEach((game, index) => {
    const card = document.createElement("div");
    card.className = "card";
    card.style.display = "flex";
    card.style.gap = "15px";
    card.style.alignItems = "center";

    card.innerHTML = `
      <img src="${game.image || 'gmbar/default.jpg'}" alt="${game.name}" 
           style="width:120px; height:80px; object-fit:cover; border-radius:4px; flex-shrink:0;">
      
      <div style="flex:1;">
        <strong style="font-size:16px;">#${index + 1}. ${game.name}</strong><br>
        <span style="color:#aaa;">Publisher: ${game.studioName}</span><br>
        <span style="color:#4ea8de; font-weight:bold;">Rp ${game.price.toLocaleString('id-ID')}</span> | 
        <span style="color:#888;">${game.category}</span><br>
        <span style="font-size:12px; color:#666;">Submitted: ${game.submittedDate}</span>
      </div>
      
      <div style="display:flex; flex-direction:column; gap:8px;">
        <button onclick="approveGame(${index})" style="background:#2ecc71; padding:10px 20px;">
          ✅ Setujui
        </button>
        <button onclick="rejectGame(${index})" style="background:#e63946; padding:10px 20px;">
          ❌ Tolak
        </button>
      </div>
    `;
    div.appendChild(card);
  });
}

// ===== APPROVE GAME =====
function approveGame(index) {
  let publisherQueue = JSON.parse(localStorage.getItem("publisherQueue")) || [];
  const game = publisherQueue[index];

  if (!game) {
    alert("Game tidak ditemukan!");
    return;
  }

  // Tambahkan ke games
  let games = JSON.parse(localStorage.getItem("games")) || [];
  const newGame = {
    id: game.id,
    name: game.name,
    price: game.price,
    category: game.category,
    publisher: game.publisher,
    studioName: game.studioName,
    rating: 0,
    salesCount: 0,
    image: game.image || "gmbar/default.jpg"
  };
  games.push(newGame);
  localStorage.setItem("games", JSON.stringify(games));

  // Hapus dari queue
  publisherQueue.splice(index, 1);
  localStorage.setItem("publisherQueue", JSON.stringify(publisherQueue));

  alert(`Game "${game.name}" berhasil disetujui dan dipublikasikan!`);
  renderPublisherQueue();
}

// ===== REJECT GAME =====
function rejectGame(index) {
  const reason = prompt("Alasan penolakan:");
  if (!reason) return;

  let publisherQueue = JSON.parse(localStorage.getItem("publisherQueue")) || [];
  const game = publisherQueue[index];

  if (!game) {
    alert("Game tidak ditemukan!");
    return;
  }

  // Save rejection notification for publisher
  let rejections = JSON.parse(localStorage.getItem("rejections")) || [];
  rejections.push({
    publisher: game.publisher,
    gameName: game.name,
    studioName: game.studioName,
    reason: reason,
    timestamp: new Date().toISOString(),
    status: 'unread'
  });
  localStorage.setItem("rejections", JSON.stringify(rejections));

  alert(`Game "${game.name}" ditolak. Alasan: ${reason}`);

  publisherQueue.splice(index, 1);
  localStorage.setItem("publisherQueue", JSON.stringify(publisherQueue));
  renderPublisherQueue();
}

// ===== INIT =====
renderQueue();
renderStack();
renderPublisherQueue();
