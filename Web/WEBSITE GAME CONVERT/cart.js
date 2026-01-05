

function renderCart() {
    const cartGrid = document.getElementById("cartGrid");
    const totalPriceElement = document.getElementById("totalPrice");

    // Loading state
    cartGrid.innerHTML = "<p style='text-align:center;'>⏳ Memuat keranjang...</p>";

    const games = JSON.parse(localStorage.getItem("games")) || [];
    const currentUserUsername = localStorage.getItem("currentUser");

    // 1. Validasi Login
    if (!currentUserUsername) {
        cartGrid.innerHTML = "<p style='text-align:center;'>Silakan login untuk melihat keranjang.</p>";
        return;
    }

    // 2. Ambil data User dari Database Users
    const users = JSON.parse(localStorage.getItem("users")) || [];
    const user = users.find(u => u.username === currentUserUsername);

    if (!user || !user.cart || user.cart.length === 0) {
        cartGrid.innerHTML = "<div style='text-align:center; padding:50px;'><h3>Keranjang Kosong</h3><p>Ayo cari game seru di Store!</p></div>";
        if (totalPriceElement) totalPriceElement.innerText = "Total: Rp 0";
        return;
    }

    cartGrid.innerHTML = "";
    let total = 0;

    // 3. Render setiap Game yang ada di Cart User
    user.cart.forEach((gameId, index) => {
        const game = games.find(g => g.id === gameId);
        if (game) {
            total += game.price;
            const item = document.createElement("div");
            item.className = "cart-item";
            item.style = "display:flex; align-items:center; gap:15px; background:#2b2d42; padding:15px; margin-bottom:10px; border-radius:8px; border: 1px solid #3d3f5e;";

            item.innerHTML = `
                <img src="${game.image}" alt="${game.name}" style="width:100px; height:70px; min-width:100px; max-width:100px; object-fit:cover; border-radius:4px; flex-shrink:0;">
                <div style="flex:1;">
                    <h4 style="margin:0; color:#fff;">${game.name}</h4>
                    <p style="margin:5px 0 0 0; color:#4ea8de; font-weight:bold;">Rp ${game.price.toLocaleString('id-ID')}</p>
                </div>
                <button onclick="removeFromCart(${index})" style="background:#e63946; color:white; border:none; padding:8px 12px; border-radius:4px; cursor:pointer; font-weight:bold; flex-shrink:0;">Hapus</button>
            `;
            cartGrid.appendChild(item);
        }
    });

    if (totalPriceElement) {
        totalPriceElement.innerText = `Total: Rp ${total.toLocaleString('id-ID')}`;
    }
}

// Fungsi Hapus Item dari Keranjang
function removeFromCart(index) {
    const currentUserUsername = localStorage.getItem("currentUser");
    let users = JSON.parse(localStorage.getItem("users")) || [];
    let userIndex = users.findIndex(u => u.username === currentUserUsername);

    if (userIndex !== -1) {
        users[userIndex].cart.splice(index, 1);
        localStorage.setItem("users", JSON.stringify(users));
        renderCart(); // Refresh tampilan
    }
}

// FUNGSI CHECKOUT: Memindahkan game dari Cart ke Library
function checkout() {
    const currentUserUsername = localStorage.getItem("currentUser");

    if (!currentUserUsername) {
        alert("Silakan login terlebih dahulu!");
        return;
    }

    let users = JSON.parse(localStorage.getItem("users")) || [];
    let userIndex = users.findIndex(u => u.username === currentUserUsername);

    if (userIndex === -1) {
        alert("User tidak ditemukan!");
        return;
    }

    if (!users[userIndex].cart || users[userIndex].cart.length === 0) {
        alert("Keranjang masih kosong!");
        return;
    }

    // Prevent double-checkout dengan flag
    if (users[userIndex].isProcessing) {
        alert("Checkout sedang diproses, tunggu sebentar!");
        return;
    }

    // Set flag processing
    users[userIndex].isProcessing = true;
    localStorage.setItem("users", JSON.stringify(users));

    try {
        const games = JSON.parse(localStorage.getItem("games")) || [];
        const user = users[userIndex];
        const isPremium = user.membershipLevel === "premium";
        const discount = isPremium ? 0.3 : 0;

        // Generate receipts for each game
        const receiptsKey = `receipts_${user.username}`;
        let receipts = JSON.parse(localStorage.getItem(receiptsKey)) || [];

        // Pindahkan dari cart ke library
        users[userIndex].cart.forEach(gameId => {
            if (!users[userIndex].library.includes(gameId)) {
                users[userIndex].library.push(gameId);

                // Generate receipt
                const game = games.find(g => g.id === gameId);
                if (game) {
                    const transactionId = `TRX${Date.now()}${Math.floor(Math.random() * 10000)}`;
                    const receipt = {
                        transactionId: transactionId,
                        timestamp: new Date().toISOString(),
                        username: user.username,
                        game: game.name,
                        gameId: game.id,
                        originalPrice: game.price,
                        discount: discount,
                        finalPrice: Math.floor(game.price * (1 - discount)),
                        savings: Math.floor(game.price * discount),
                        paymentMethod: getRandomPaymentMethod()
                    };
                    receipts.push(receipt);

                    // Increment game sales count
                    if (typeof incrementGameSales === "function") {
                        incrementGameSales(gameId);
                    }
                }
            }
        });

        // Save receipts
        localStorage.setItem(receiptsKey, JSON.stringify(receipts));

        // Kosongkan cart
        users[userIndex].cart = [];

        // Reset flag
        users[userIndex].isProcessing = false;

        // Save ke localStorage
        localStorage.setItem("users", JSON.stringify(users));

        const message = isPremium
            ? `Checkout Berhasil dengan Diskon 30%!\n\nGame telah ditambahkan ke Library Anda.\nStruk pembelian tersimpan di menu Receipts.`
            : `Checkout Berhasil!\n\nGame telah ditambahkan ke Library Anda.\n\nUpgrade ke Premium untuk diskon 30% di semua pembelian!`;

        alert(message);
        window.location.href = "library.html";

    } catch (error) {
        console.error("Checkout error:", error);

        // Reset flag jika error
        users[userIndex].isProcessing = false;
        localStorage.setItem("users", JSON.stringify(users));

        alert("Terjadi kesalahan saat checkout. Silakan coba lagi!");
    }
}

function getRandomPaymentMethod() {
    const methods = [
        "QRIS - DANA",
        "QRIS - GoPay",
        "Transfer Bank BCA",
        "Transfer Bank Mandiri",
        "E-Wallet ShopeePay",
        "Kartu Kredit Visa",
        "Kartu Debit BCA"
    ];
    return methods[Math.floor(Math.random() * methods.length)];
}

document.addEventListener("DOMContentLoaded", renderCart);