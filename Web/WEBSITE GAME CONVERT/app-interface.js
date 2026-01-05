/**
 * APP-INTERFACE.JS
 * Menangani Animasi, Ikon, dan Transisi UI
 */

const AppInterface = {
    init() {
        this.renderIcons();
        this.setupPageTransitions();
        this.initUserUI();
    },

    // 1. Me-render ikon dari library Lucide
    renderIcons() {
        if (typeof lucide !== 'undefined') {
            lucide.createIcons();
        }
    },

    // 2. Animasi halus saat berpindah halaman
    setupPageTransitions() {
        const overlay = document.getElementById('pageOverlay');
        const links = document.querySelectorAll('.nav-item, .brand');

        links.forEach(link => {
            link.addEventListener('click', (e) => {
                const href = link.getAttribute('href');
                if (href && href !== '#' && !href.startsWith('javascript')) {
                    e.preventDefault();
                    overlay.style.opacity = '1';
                    setTimeout(() => {
                        window.location.href = href;
                    }, 400);
                }
            });
        });
    },

    // 3. Update Navbar dengan Ikon (Sinkron dengan user.js)
    initUserUI() {
        const userArea = document.getElementById("userArea");
        const user = typeof getCurrentUser === "function" ? getCurrentUser() : null;

        if (user && userArea) {
            userArea.innerHTML = `
                <div style="display:flex; align-items:center; gap:15px;">
                    <div style="background:rgba(99,102,241,0.1); padding:8px 15px; border-radius:10px; color:#6366f1; font-weight:bold; display:flex; align-items:center; gap:8px;">
                        <i data-lucide="user" style="width:16px"></i> ${user.username.toUpperCase()}
                    </div>
                    <button onclick="logout()" style="background:none; border:none; color:#ef4444; cursor:pointer; display:flex; align-items:center; gap:5px; font-weight:600;">
                        <i data-lucide="log-out" style="width:16px"></i> Keluar
                    </button>
                </div>
            `;
            this.renderIcons();
        }
    }
};

// Jalankan saat dokumen siap
document.addEventListener("DOMContentLoaded", () => AppInterface.init());