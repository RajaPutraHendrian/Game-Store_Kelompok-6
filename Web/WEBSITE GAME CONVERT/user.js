/* =========================================
   USER SYSTEM – UPDATED FULL VERSION
   Logic: Auth, Cart, Wishlist, Library, Queue
   Storage: localStorage
========================================= */


function safeParseJSON(key, defaultValue = []) {
  try {
    const data = localStorage.getItem(key);
    if (!data) return defaultValue;
    return JSON.parse(data);
  } catch (error) {
    console.error(`Error parsing ${key}:`, error);
    localStorage.setItem(key, JSON.stringify(defaultValue));
    return defaultValue;
  }
}

function safeSaveJSON(key, value) {
  try {
    localStorage.setItem(key, JSON.stringify(value));
    return true;
  } catch (error) {
    console.error(`Error saving ${key}:`, error);
    return false;
  }
}

// Initialize storage dengan safe parser
if (!localStorage.getItem("users")) {
  localStorage.setItem("users", JSON.stringify([]));
}
if (!localStorage.getItem("currentUser")) {
  localStorage.setItem("currentUser", "");
}
if (!localStorage.getItem("purchaseQueue")) {
  localStorage.setItem("purchaseQueue", JSON.stringify([]));
}
if (!localStorage.getItem("transactionStack")) {
  localStorage.setItem("transactionStack", JSON.stringify([]));
}

let users = safeParseJSON("users", []);

/* ========= DEFAULT ADMIN ========= */
if (!users.find(u => u.username === "admin")) {
  users.push({
    username: "admin",
    password: "admin123",
    role: "admin",
    wishlist: [],
    cart: [],
    library: []
  });
  saveUsers();
}

/* ========= HELPER FUNCTIONS ========= */
function saveUsers() {
  return safeSaveJSON("users", users);
}

function getCurrentUser() {
  const username = localStorage.getItem("currentUser");
  return users.find(u => u.username === username);
}

/* ========= AUTH SYSTEM ========= */
function register(username, password) {
  if (users.find(u => u.username === username)) {
    alert("Username sudah terdaftar");
    return false;
  }
  users.push({
    username,
    password,
    role: "user",
    wishlist: [],
    cart: [],
    library: [],
    membershipLevel: "free",  // New: free or premium
    downloadedGames: []  // New: downloaded games list
  });
  saveUsers();
  return true;
}

// ===== PUBLISHER REGISTRATION =====
function registerPublisher(username, password, studioName, country, yearFounded) {
  if (users.find(u => u.username === username)) {
    alert("Username sudah terdaftar");
    return false;
  }
  users.push({
    username,
    password,
    role: "publisher",
    studioName,
    country,
    yearFounded,
    wishlist: [],
    cart: [],
    library: [],
    membershipLevel: "free",
    downloadedGames: [],
    pendingGames: [],  // Game menunggu verifikasi
    approvedGames: []  // Game yang sudah disetujui
  });
  saveUsers();
  return true;
}

// ===== PUBLISHER: Submit Game untuk Verifikasi =====
function submitGameForApproval(gameName, price, category, imageFile) {
  const user = getCurrentUser();
  if (!user || user.role !== "publisher") {
    alert("Hanya Publisher yang bisa submit game");
    return false;
  }

  // Tambahkan ke pending queue
  let queue = JSON.parse(localStorage.getItem("publisherQueue")) || [];

  const newGame = {
    id: Date.now(), // ID unik
    name: gameName,
    price: price,
    category: category,
    publisher: user.username,
    studioName: user.studioName,
    status: "pending",
    submittedDate: new Date().toLocaleString(),
    image: imageFile || "gmbar/default.jpg" // Default jika tidak ada gambar
  };

  queue.push(newGame);
  localStorage.setItem("publisherQueue", JSON.stringify(queue));

  // Tambahkan ke pending games publisher
  user.pendingGames.push(newGame.id);
  saveUsers();

  alert("Game berhasil diajukan untuk verifikasi!");
  return true;
}

// ===== PUBLISHER: Lihat Game yang Disetujui =====
function getApprovedGames() {
  const user = getCurrentUser();
  if (!user || user.role !== "publisher") return [];

  const allGames = JSON.parse(localStorage.getItem("games")) || [];
  return allGames.filter(g => g.publisher === user.username);
}

// ===== PUBLISHER: Lihat Pending Games =====
function getPendingGames() {
  const user = getCurrentUser();
  if (!user || user.role !== "publisher") return [];

  const queue = JSON.parse(localStorage.getItem("publisherQueue")) || [];
  return queue.filter(g => g.publisher === user.username);
}

// ===== PUBLISHER: Update Game Data =====
function updatePublisherGame(gameId, newName, newPrice, newCategory) {
  const user = getCurrentUser();
  if (!user || user.role !== "publisher") return false;

  let games = JSON.parse(localStorage.getItem("games")) || [];
  const gameIndex = games.findIndex(g => g.id === gameId && g.publisher === user.username);

  if (gameIndex === -1) {
    alert("Game tidak ditemukan atau bukan milik Anda");
    return false;
  }

  games[gameIndex].name = newName;
  games[gameIndex].price = newPrice;
  games[gameIndex].category = newCategory;

  localStorage.setItem("games", JSON.stringify(games));
  alert("Game berhasil diupdate!");
  return true;
}

function login(username, password) {
  const user = users.find(u => u.username === username && u.password === password);
  if (!user) {
    alert("Username atau password salah");
    return false;
  }
  localStorage.setItem("currentUser", username);
  return true;
}

function logout() {
  localStorage.setItem("currentUser", "");
  window.location.href = "index.html";
}

/* ========= WISHLIST & CART ========= */
function addToWishlist(gameId) {
  const user = getCurrentUser();
  if (!user) return alert("Silakan login terlebih dahulu");

  // Prevent adding owned games to wishlist
  if (user.library.includes(gameId)) {
    alert("Game sudah Anda miliki!");
    return false;
  }

  if (!user.wishlist.includes(gameId)) {
    user.wishlist.push(gameId);
    saveUsers();
    alert("Berhasil masuk wishlist");
    return true;
  } else {
    alert("Game sudah ada di wishlist");
    return false;
  }
}

function addToCart(gameId) {
  const user = getCurrentUser();
  if (!user) return alert("Silakan login terlebih dahulu");

  if (user.library.includes(gameId)) return alert("Anda sudah memiliki game ini");
  if (user.cart.includes(gameId)) return alert("Game sudah ada di keranjang");

  user.cart.push(gameId);
  saveUsers();
  alert("Berhasil masuk keranjang");
}

function removeFromCart(gameId) {
  const user = getCurrentUser();
  if (!user) return;
  user.cart = user.cart.filter(id => id !== gameId);
  saveUsers();
}

/* ========= CHECKOUT (QUEUE SYSTEM) ========= */
// Saat user klik checkout, data masuk ke antrean Admin
function checkoutCart() {
  const user = getCurrentUser();
  if (!user || user.cart.length === 0) return alert("Keranjang kosong");

  const queue = JSON.parse(localStorage.getItem("purchaseQueue")) || [];

  user.cart.forEach(gameId => {
    // Masukkan ke antrean admin (FIFO)
    queue.push({
      username: user.username,
      gameId: gameId,
      date: new Date().toLocaleString()
    });
  });

  localStorage.setItem("purchaseQueue", JSON.stringify(queue));

  // Kosongkan keranjang user setelah checkout
  user.cart = [];
  saveUsers();

  alert("Checkout Berhasil! Pesanan sedang diproses oleh Admin.");
  if (typeof renderCart === "function") renderCart(); // Refresh tampilan jika di halaman cart
}

/* ========= DOWNLOAD (LIBRARY) ========= */
function downloadGame(gameId) {
  const user = getCurrentUser();
  if (!user) return;

  const key = `installed_${user.username}_${gameId}`;
  localStorage.setItem(key, "true");
  alert("Game sedang diunduh...");
  if (typeof renderLibrary === "function") renderLibrary();
}

/* ========= ADMIN LOGIC ========= */
function isAdmin() {
  const user = getCurrentUser();
  return user && user.role === "admin";
}

/* ========= BROWSING HISTORY (STACK) ========= */
function addToBrowsingHistory(gameId) {
  const user = getCurrentUser();
  if (!user) return;

  const historyKey = `browsingHistory_${user.username}`;
  let history = JSON.parse(localStorage.getItem(historyKey)) || [];

  // Remove if already exists (avoid duplicates)
  history = history.filter(item => item.gameId !== gameId);

  // Add to top of stack (most recent)
  history.push({
    gameId: gameId,
    timestamp: new Date().toISOString()
  });

  // Limit to 20 items
  if (history.length > 20) {
    history.shift(); // Remove oldest
  }

  localStorage.setItem(historyKey, JSON.stringify(history));
}

/* ========= DOWNLOAD MANAGEMENT ========= */
function addToDownloaded(gameId) {
  const user = getCurrentUser();
  if (!user) return alert("Silakan login terlebih dahulu");

  if (!user.library.includes(gameId)) {
    return alert("Anda belum membeli game ini!");
  }

  let users = JSON.parse(localStorage.getItem("users")) || [];
  const userIndex = users.findIndex(u => u.username === user.username);

  if (userIndex === -1) return;

  if (!users[userIndex].downloadedGames) {
    users[userIndex].downloadedGames = [];
  }

  if (users[userIndex].downloadedGames.includes(gameId)) {
    alert("Game sudah terdownload!");
    return;
  }

  users[userIndex].downloadedGames.push(gameId);
  localStorage.setItem("users", JSON.stringify(users));
  alert("Game berhasil didownload!");
}

function removeFromDownloaded(gameId) {
  const user = getCurrentUser();
  if (!user) return;

  let users = JSON.parse(localStorage.getItem("users")) || [];
  const userIndex = users.findIndex(u => u.username === user.username);

  if (userIndex === -1) return;

  if (!users[userIndex].downloadedGames) {
    users[userIndex].downloadedGames = [];
  }

  users[userIndex].downloadedGames = users[userIndex].downloadedGames.filter(id => id !== gameId);
  localStorage.setItem("users", JSON.stringify(users));
  alert("Game berhasil dihapus dari download!");
}

function isGameDownloaded(gameId) {
  const user = getCurrentUser();
  if (!user || !user.downloadedGames) return false;
  return user.downloadedGames.includes(gameId);
}

/* ========= MEMBERSHIP HELPERS ========= */
function isPremiumMember() {
  const user = getCurrentUser();
  return user && user.membershipLevel === "premium";
}

function getDiscountRate() {
  return isPremiumMember() ? 0.3 : 0; // 30% for premium, 0% for free
}

function applyMembershipDiscount(price) {
  const discount = getDiscountRate();
  return Math.floor(price * (1 - discount));
}
