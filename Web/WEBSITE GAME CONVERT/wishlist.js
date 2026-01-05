const grid = document.getElementById("wishlistGrid");

// Ambil data game
const games = JSON.parse(localStorage.getItem("games")) || [];

/* ========= XSS PROTECTION ========= */
function escapeHTML(text) {
  const div = document.createElement('div');
  div.textContent = text;
  return div.innerHTML;
}

/* ===============================
   RENDER WISHLIST
================================ */
function renderWishlist() {
  // Reload users from localStorage to get fresh data
  if (typeof users !== 'undefined') {
    users = JSON.parse(localStorage.getItem("users")) || [];
  }

  const user = getCurrentUser();

  // Jika belum login
  if (!user) {
    grid.innerHTML = `<div class="message">Silakan login terlebih dahulu.</div>`;
    return;
  }

  // Jika wishlist kosong
  if (!user.wishlist || user.wishlist.length === 0) {
    grid.innerHTML = `<div class="message">Wishlist kosong.</div>`;
    return;
  }

  grid.innerHTML = "";

  user.wishlist.forEach(id => {
    const game = games.find(g => g.id === id);
    if (!game) return;

    const card = document.createElement("div");
    card.className = "card";

    card.innerHTML = `
      <h4>${escapeHTML(game.name)}</h4>
      <div>${game.price === 0 ? "Free" : "Rp " + game.price}</div>
      <button class="cart" onclick="addToCartFromWishlist(${id})">Add to Cart</button>
      <button class="remove" onclick="removeFromWishlist(${id})">Remove</button>
    `;

    grid.appendChild(card);
  });
}

/* ===============================
   HAPUS DARI WISHLIST
================================ */
function removeFromWishlist(gameId) {
  const currentUsername = localStorage.getItem("currentUser");
  if (!currentUsername) return;

  // Update data di localStorage users
  let users = JSON.parse(localStorage.getItem("users")) || [];
  const userIndex = users.findIndex(u => u.username === currentUsername);

  if (userIndex !== -1) {
    users[userIndex].wishlist = users[userIndex].wishlist.filter(id => id !== gameId);
    localStorage.setItem("users", JSON.stringify(users));

    // FIX: Reload global users variable dari user.js
    if (typeof users !== 'undefined' && window.users) {
      window.users = users;
    }
  }

  // Render ulang wishlist - INI SUDAH BENAR
  renderWishlist();
}

/* ===============================
   TAMBAH KE KERANJANG DARI WISHLIST
================================ */
function addToCartFromWishlist(gameId) {
  const currentUsername = localStorage.getItem("currentUser");
  if (!currentUsername) return;

  let users = JSON.parse(localStorage.getItem("users")) || [];
  const userIndex = users.findIndex(u => u.username === currentUsername);

  if (userIndex === -1) {
    alert("User tidak ditemukan!");
    return;
  }

  // Cek apakah sudah ada di library
  if (users[userIndex].library && users[userIndex].library.includes(gameId)) {
    alert("Anda sudah memiliki game ini di Library!");
    return;
  }

  // Inisialisasi cart jika belum ada
  if (!users[userIndex].cart) {
    users[userIndex].cart = [];
  }

  // Cek apakah sudah ada di cart
  if (users[userIndex].cart.includes(gameId)) {
    alert("Game sudah ada di keranjang!");
    return;
  }

  // Tambahkan ke cart
  users[userIndex].cart.push(gameId);

  // Hapus dari wishlist setelah ditambahkan ke cart
  users[userIndex].wishlist = users[userIndex].wishlist.filter(id => id !== gameId);

  localStorage.setItem("users", JSON.stringify(users));

  // Update global users variable
  if (typeof window.users !== 'undefined') {
    window.users = users;
  }

  alert("Game ditambahkan ke keranjang!");

  // Render ulang wishlist untuk update data terbaru
  renderWishlist();
}

/* ===============================
   AUTO LOAD
================================ */
renderWishlist();