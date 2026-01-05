/* =========================================
   STORE LOGIC – FULL VERSION
========================================= */

const defaultGames = [
  {
    id: 1,
    name: "Pong",
    price: 53000,
    category: "Adventure",
    rating: 4.0,
    image: "gmbar/pong.png"
  },
  {
    id: 2,
    name: "Space Invaders",
    price: 20000,
    category: "RPG",
    rating: 4.5,
    image: "gmbar/space.jpeg"
  },
  {
    id: 3,
    name: "Pac-Man",
    price: 0,
    category: "Retro",
    rating: 4.8,
    image: "gmbar/pacman.jpg"
  },
  {
    id: 4,
    name: "Donkey Kong",
    price: 50000,
    category: "RPG",
    rating: 4.6,
    image: "gmbar/donkey.png"
  },
  {
    id: 5,
    name: "Tetris",
    price: 0,
    category: "Retro",
    rating: 4.9,
    image: "gmbar/tetris.png"
  },
  {
    id: 6,
    name: "Super Mario Bros.",
    price: 24000,
    category: "RPG",
    rating: 4.9,
    image: "gmbar/mario.png"
  },
  {
    id: 7,
    name: "The Legend of Zelda",
    price: 10000,
    category: "RPG",
    rating: 4.8,
    image: "gmbar/zelda.png"
  },
  {
    id: 8,
    name: "Metroid",
    price: 15000,
    category: "Retro",
    rating: 4.7,
    image: "gmbar/metroid.png"
  },
  {
    id: 9,
    name: "Final Fantasy",
    price: 54200,
    category: "Action",
    rating: 4.5,
    image: "gmbar/ff1.png"
  },
  {
    id: 10,
    name: "Mega Man 2",
    price: 21000,
    category: "Action",
    rating: 4.7,
    image: "gmbar/megaman.png"
  },
  {
    id: 11,
    name: "SimCity",
    price: 0,
    category: "Simulator",
    rating: 4.6,
    image: "gmbar/simcity.png"
  },
  {
    id: 12,
    name: "Street Fighter II",
    price: 50000,
    category: "Action",
    rating: 4.8,
    image: "gmbar/sf2.png"
  },
  {
    id: 13,
    name: "Sonic the Hedgehog",
    price: 0,
    category: "Adventure,Action",
    rating: 4.7,
    image: "gmbar/sonic.png"
  },
  {
    id: 14,
    name: "Doom",
    price: 15000,
    category: "Adventure",
    rating: 4.8,
    image: "gmbar/doom.png"
  },
  {
    id: 15,
    name: "Myst",
    price: 10000,
    category: "Adventure",
    rating: 4.3,
    image: "gmbar/myst.png"
  },
  {
    id: 16,
    name: "Warcraft: Orcs & Humans",
    price: 10000,
    category: "Adventure",
    rating: 4.4,
    image: "gmbar/warcraft.png"
  },
  {
    id: 17,
    name: "Super Mario 64",
    price: 67000,
    category: "Adventure,Action",
    rating: 4.9,
    image: "gmbar/mario64.png"
  },
  {
    id: 18,
    name: "GoldenEye 007",
    price: 15000,
    category: "RPG",
    rating: 4.7,
    image: "gmbar/goldeneye.png"
  },
  {
    id: 19,
    name: "Half-Life",
    price: 10050,
    category: "Simulator",
    rating: 4.8,
    image: "gmbar/halflife.png"
  },
  {
    id: 20,
    name: "The Legend of Zelda: Ocarina of Time",
    price: 20000,
    category: "Adventure,Action",
    rating: 5.0,
    image: "gmbar/zelda_oot.png"
  },
  {
    id: 21,
    name: "Metal Gear Solid",
    price: 18000,
    category: "Action,Stealth",
    rating: 4.8,
    image: "gmbar/mgs.jpg"
  },
  {
    id: 22,
    name: "Final Fantasy VII",
    price: 22000,
    category: "RPG",
    rating: 4.9,
    image: "gmbar/ff7.png"
  },
  {
    id: 23,
    name: "StarCraft",
    price: 16000,
    category: "Strategy",
    rating: 4.8,
    image: "gmbar/starcraft.png"
  },
  {
    id: 24,
    name: "Resident Evil 2",
    price: 19000,
    category: "Horror,Action",
    rating: 4.7,
    image: "gmbar/re2.png"
  },
  {
    id: 25,
    name: "Silent Hill",
    price: 17000,
    category: "Horror",
    rating: 4.6,
    image: "gmbar/silenthill.png"
  },
  {
    id: 26,
    name: "Diablo II",
    price: 21000,
    category: "RPG,Action",
    rating: 4.8,
    image: "gmbar/diablo2.png"
  },
  {
    id: 27,
    name: "The Sims",
    price: 0,
    category: "Simulator",
    rating: 4.6,
    image: "gmbar/sims.png"
  },
  {
    id: 28,
    name: "Grand Theft Auto III",
    price: 23000,
    category: "Action,Open World",
    rating: 4.7,
    image: "gmbar/gta3.png"
  },
  {
    id: 29,
    name: "Halo: Combat Evolved",
    price: 20000,
    category: "Shooter",
    rating: 4.8,
    image: "gmbar/halo.png"
  },
  {
    id: 30,
    name: "World of Warcraft",
    price: 25000,
    category: "MMORPG",
    rating: 4.7,
    image: "gmbar/wow.png"
  },
  {
    id: 31,
    name: "Half-Life 2",
    price: 23000,
    category: "Shooter",
    rating: 4.9,
    image: "gmbar/hl2.png"
  },
  {
    id: 32,
    name: "Shadow of the Colossus",
    price: 24000,
    category: "Adventure",
    rating: 4.8,
    image: "gmbar/sotc.png"
  },
  {
    id: 33,
    name: "BioShock",
    price: 20000,
    category: "Shooter",
    rating: 4.8,
    image: "gmbar/bioshock.png"
  },
  {
    id: 34,
    name: "Portal",
    price: 15000,
    category: "Puzzle",
    rating: 4.9,
    image: "gmbar/portal.png"
  },
  {
    id: 35,
    name: "Mass Effect",
    price: 21000,
    category: "RPG,Sci-Fi",
    rating: 4.7,
    image: "gmbar/masseffect.png"
  },
  {
    id: 36,
    name: "Fallout 3",
    price: 20000,
    category: "RPG",
    rating: 4.6,
    image: "gmbar/fallout3.png"
  },
  {
    id: 37,
    name: "Uncharted: Drake's Fortune",
    price: 19000,
    category: "Adventure,Action",
    rating: 4.5,
    image: "gmbar/uncharted.png"
  },
  {
    id: 38,
    name: "Red Dead Redemption",
    price: 25000,
    category: "Action,Adventure",
    rating: 4.8,
    image: "gmbar/rdr.png"
  },
  {
    id: 39,
    name: "The Elder Scrolls V: Skyrim",
    price: 27000,
    category: "RPG,Open World",
    rating: 4.8,
    image: "gmbar/skyrim.png"
  },
  {
    id: 40,
    name: "Dark Souls",
    price: 26000,
    category: "RPG,Action",
    rating: 4.7,
    image: "gmbar/darksouls.png"
  },
  {
    id: 41,
    name: "The Witcher 3: Wild Hunt",
    price: 30000,
    category: "RPG,Open World",
    rating: 4.9,
    image: "gmbar/witcher3.png"
  },
  {
    id: 42,
    name: "Overwatch",
    price: 25000,
    category: "Shooter,Multiplayer",
    rating: 4.5,
    image: "gmbar/overwatch.png"
  },
  {
    id: 43,
    name: "Fortnite",
    price: 0,
    category: "Battle Royale",
    rating: 4.3,
    image: "gmbar/fortnite.png"
  },
  {
    id: 44,
    name: "Minecraft",
    price: 0,
    category: "Sandbox,Survival",
    rating: 4.8,
    image: "gmbar/minecraft.png"
  },
  {
    id: 45,
    name: "Roblox",
    price: 0,
    category: "Online,Sandbox",
    rating: 4.2,
    image: "gmbar/roblox.png"
  },
  {
    id: 46,
    name: "Valorant",
    price: 0,
    category: "Shooter,Tactical",
    rating: 4.4,
    image: "gmbar/valorant.png"
  },
  {
    id: 47,
    name: "Red Dead Redemption 2",
    price: 35000,
    category: "Action,Adventure",
    rating: 4.9,
    image: "gmbar/rdr2.png"
  },
  {
    id: 48,
    name: "Cyberpunk 2077",
    price: 35000,
    category: "RPG,Sci-Fi",
    rating: 4.2,
    image: "gmbar/cyberpunk.png"
  },
  {
    id: 49,
    name: "Iron Clash",
    price: 17296,
    category: "Action",
    rating: 4.1,
    image: "gmbar/ironclash.png"
  },
  {
    id: 50,
    name: "Blood Horizon",
    price: 11639,
    category: "Action",
    rating: 3.9,
    image: "gmbar/bloodhorizon.png"
  },
  {
    id: 51,
    name: "Cyberpunk Edge",
    price: 0,
    category: "Action",
    rating: 4.8,
    // Mengarah ke folder: gmbar/nama_file.jpg
    image: "gmbar/cyberpunkedge.jpg"
  },
  {
    id: 52,
    name: "Fantasy Realm",
    price: 125000,
    category: "RPG",
    rating: 4.5,
    image: "gmbar/fantasy.jpg"
  },
  {
    id: 53,
    name: "Lost in Space",
    price: 75000,
    category: "Adventure",
    rating: 4.7,
    image: "gmbar/lost.jpg"
  },
  {
    id: 54,
    name: "Delta Force",
    price: 45000,
    category: "Strategy",
    rating: 4.9,
    image: "gmbar/delta.jpg"
  },
  {
    id: 55,
    name: "Neon Racer",
    price: 25000,
    category: "Racing",
    rating: 4.4,
    image: "gmbar/rece.jpg"
  },
  {
    id: 56,
    name: "Warzone Delta",
    price: 28000,
    category: "Action",
    rating: 4.4,
    image: "gmbar/warzonedelta.png"
  },
  {
    id: 57,
    name: "Counter-Strike 2",
    price: 0,
    category: "Shooter,Multiplayer",
    rating: 4.8,
    image: "gmbar/cs2.png"
  },
  {
    id: 58,
    name: "Dota 2",
    price: 0,
    category: "MOBA",
    rating: 4.7,
    image: "gmbar/dota2.png"
  },
  {
    id: 59,
    name: "PUBG: Battlegrounds",
    price: 0,
    category: "Battle Royale",
    rating: 4.4,
    image: "gmbar/pubg.jpg"
  },
  {
    id: 60,
    name: "Apex Legends",
    price: 0,
    category: "Battle Royale",
    rating: 4.5,
    image: "gmbar/apex.jpg"
  },
  {
    id: 61,
    name: "Grand Theft Auto V",
    price: 30000,
    category: "Action,Open World",
    rating: 4.9,
    image: "gmbar/gtav.png"
  },
  {
    id: 62,
    name: "Ghost of Tsushima",
    price: 35000,
    category: "Action,Adventure",
    rating: 4.9,
    image: "gmbar/ghostoftsushima.png"
  },
  {
    id: 63,
    name: "Starfield",
    price: 40000,
    category: "RPG,Space",
    rating: 4.3,
    image: "gmbar/starfield.png"
  },
  {
    id: 64,
    name: "Elden Ring",
    price: 45000,
    category: "RPG,Action",
    rating: 4.9,
    image: "gmbar/eldenring.png"
  },
  {
    id: 65,
    name: "Baldur's Gate 3",
    price: 55000,
    category: "RPG",
    rating: 5.0,
    image: "gmbar/bg3.jpg"
  },
  {
    id: 66,
    name: "Dragon's Dogma 2",
    price: 45000,
    category: "RPG,Action",
    rating: 4.6,
    image: "gmbar/dragonsdogma2.png"
  },
  {
    id: 67,
    name: "Monster Hunter: World",
    price: 28000,
    category: "Action,RPG",
    rating: 4.8,
    image: "gmbar/mhw.png"
  },
  {
    id: 68,
    name: "Monster Hunter Rise",
    price: 32000,
    category: "Action,RPG",
    rating: 4.7,
    image: "gmbar/mhrise.jpg"
  },
  {
    id: 69,
    name: "Terraria",
    price: 10000,
    category: "Sandbox,Survival",
    rating: 4.9,
    image: "gmbar/terraria.png"
  },
  {
    id: 70,
    name: "Stardew Valley",
    price: 15000,
    category: "Simulation,RPG",
    rating: 4.9,
    image: "gmbar/stardew.jpg"
  },
  {
    id: 71,
    name: "Hades",
    price: 24000,
    category: "Rogue-like,Action",
    rating: 4.9,
    image: "gmbar/hades.png"
  },
  {
    id: 72,
    name: "Dead Cells",
    price: 23000,
    category: "Rogue-like,Action",
    rating: 4.8,
    image: "gmbar/deadcells.png"
  },
  {
    id: 73,
    name: "Sekiro: Shadows Die Twice",
    price: 35000,
    category: "Action",
    rating: 4.9,
    image: "gmbar/sekiro.png"
  },
  {
    id: 74,
    name: "Dark Souls III",
    price: 30000,
    category: "Action,RPG",
    rating: 4.8,
    image: "gmbar/ds3.jpg"
  },
  {
    id: 75,
    name: "Rust",
    price: 30000,
    category: "Survival,Multiplayer",
    rating: 4.4,
    image: "gmbar/rust.jpg"
  },
  {
    id: 76,
    name: "ARK: Survival Evolved",
    price: 28000,
    category: "Survival,Open World",
    rating: 4.3,
    image: "gmbar/ark.png"
  },
  {
    id: 77,
    name: "Valheim",
    price: 20000,
    category: "Survival",
    rating: 4.8,
    image: "gmbar/valheim.jpg"
  },
  {
    id: 78,
    name: "Subnautica",
    price: 26000,
    category: "Survival,Adventure",
    rating: 4.8,
    image: "gmbar/subnautica.png"
  },
  {
    id: 79,
    name: "Factorio",
    price: 30000,
    category: "Simulation,Strategy",
    rating: 4.9,
    image: "gmbar/factorio.jpg"
  },
  {
    id: 80,
    name: "Cities: Skylines",
    price: 28000,
    category: "Simulation",
    rating: 4.7,
    image: "gmbar/citiesskylines.png"
  },
  {
    id: 81,
    name: "Civilization VI",
    price: 30000,
    category: "Strategy",
    rating: 4.6,
    image: "gmbar/civ6.jpg"
  },
  {
    id: 82,
    name: "Euro Truck Simulator 2",
    price: 20000,
    category: "Simulation",
    rating: 4.8,
    image: "gmbar/ets2.png"
  },
  {
    id: 83,
    name: "Among Us",
    price: 6000,
    category: "Party,Multiplayer",
    rating: 4.5,
    image: "gmbar/amongus.png"
  },
  {
    id: 84,
    name: "Phasmophobia",
    price: 20000,
    category: "Horror,Co-op",
    rating: 4.7,
    image: "gmbar/phasmophobia.png"
  },
  {
    id: 85,
    name: "Left 4 Dead 2",
    price: 15000,
    category: "Shooter,Co-op",
    rating: 4.9,
    image: "gmbar/l4d2.png"
  },
  {
    id: 86,
    name: "Payday 2",
    price: 12000,
    category: "Shooter,Co-op",
    rating: 4.6,
    image: "gmbar/payday2.png"
  },
  {
    id: 87,
    name: "Portal 2",
    price: 10000,
    category: "Puzzle",
    rating: 4.9,
    image: "gmbar/portal2.png"
  },
  {
    id: 88,
    name: "Team Fortress 2",
    price: 0,
    category: "Shooter",
    rating: 4.7,
    image: "gmbar/tf2.png"
  },
  {
    id: 89,
    name: "Overcooked 2",
    price: 24000,
    category: "Party,Co-op",
    rating: 4.6,
    image: "gmbar/overcooked2.jpg"
  },
  {
    id: 90,
    name: "It Takes Two",
    price: 35000,
    category: "Adventure,Co-op",
    rating: 4.9,
    image: "gmbar/ittakestwo.png"
  },
  {
    id: 91,
    name: "No Man's Sky",
    price: 30000,
    category: "Adventure,Space",
    rating: 4.6,
    image: "gmbar/nomanssky.png"
  },
  {
    id: 92,
    name: "Sea of Thieves",
    price: 30000,
    category: "Adventure,Multiplayer",
    rating: 4.6,
    image: "gmbar/seaofthieves.png"
  },
  {
    id: 93,
    name: "Assassin's Creed Valhalla",
    price: 35000,
    category: "Action,RPG",
    rating: 4.4,
    image: "gmbar/acvalhalla.png"
  },
  {
    id: 94,
    name: "Hollow Knight",
    price: 15000,
    category: "Metroidvania",
    rating: 4.9,
    image: "gmbar/hollowknight.jpg"
  },
  {
    id: 95,
    name: "Cuphead",
    price: 20000,
    category: "Action",
    rating: 4.8,
    image: "gmbar/cuphead.png"
  },
  {
    id: 96,
    name: "The Forest",
    price: 18000,
    category: "Horror,Survival",
    rating: 4.5,
    image: "gmbar/theforest.png"
  },
  {
    id: 97,
    name: "Sons of the Forest",
    price: 30000,
    category: "Horror,Survival",
    rating: 4.4,
    image: "gmbar/sonsoftheforest.jpg"
  },
  {
    id: 98,
    name: "Lethal Company",
    price: 15000,
    category: "Horror,Co-op",
    rating: 4.8,
    image: "gmbar/lethalcompany.png"
  },
  {
    id: 99,
    name: "Palworld",
    price: 30000,
    category: "Survival,Open World",
    rating: 4.6,
    image: "gmbar/palworld.png"
  },
  {
    id: 100,
    name: "Helldivers 2",
    price: 40000,
    category: "Shooter,Co-op",
    rating: 4.8,
    image: "gmbar/helldivers2.png"
  }

];

/* ========= OWNERSHIP CHECK ========= */
function isOwned(gameId) {
  try {
    const currentUserUsername = localStorage.getItem("currentUser");
    if (!currentUserUsername) return false;

    const users = JSON.parse(localStorage.getItem("users")) || [];
    const user = users.find(u => u.username === currentUserUsername);

    if (!user || !user.library) return false;
    return user.library.includes(gameId);
  } catch (error) {
    console.error("Error checking ownership:", error);
    return false;
  }
}

// Logika reset otomatis agar data baru terbaca
const storedGames = localStorage.getItem("games");
if (!storedGames) {
  localStorage.setItem("games", JSON.stringify(defaultGames));
}

const games = JSON.parse(localStorage.getItem("games"));

function renderStore(list) {
  const grid = document.getElementById("gameGrid");
  if (!grid) return;
  grid.innerHTML = "";

  list.forEach(game => {
    const owned = typeof isOwned === "function" ? isOwned(game.id) : false;
    const card = document.createElement("div");
    card.style.background = "#2b2d42";
    card.style.padding = "15px";
    card.style.borderRadius = "8px";
    card.style.cursor = "pointer";
    card.style.transition = "transform 0.3s, box-shadow 0.3s";

    card.addEventListener("mouseenter", () => {
      card.style.transform = "translateY(-5px)";
      card.style.boxShadow = "0 8px 20px rgba(78, 168, 222, 0.3)";
    });

    card.addEventListener("mouseleave", () => {
      card.style.transform = "translateY(0)";
      card.style.boxShadow = "none";
    });

    // Open modal on click
    card.addEventListener("click", () => {
      showQuickActionModal(game.id);
      // Add to browsing history when viewing
      if (typeof addToBrowsingHistory === "function") {
        addToBrowsingHistory(game.id);
      }
    });

    // Create image element with error handling
    const img = document.createElement("img");
    img.src = game.image || "gmbar/default.jpg";
    img.alt = escapeHTML(game.name);
    img.style.cssText = "width:100%; height:150px; object-fit:cover; border-radius:4px;";

    // Fallback for missing images
    img.onerror = function () {
      this.src = "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='200' height='150' viewBox='0 0 200 150'%3E%3Crect fill='%233a3d5c' width='200' height='150'/%3E%3Ctext x='50%25' y='50%25' dominant-baseline='middle' text-anchor='middle' font-family='Arial' font-size='14' fill='%23999'%3ENo Image%3C/text%3E%3C/svg%3E";
      console.warn(`Image not found for game: ${game.name} (${game.image})`);
    };

    const contentDiv = document.createElement("div");
    contentDiv.innerHTML = `
      <h4 style="margin:10px 0 5px 0">${escapeHTML(game.name)}</h4>
      <p style="font-size:12px; color:#aaa">${escapeHTML(game.category)} | Rating: ${game.rating}</p>
      <p style="font-weight:bold; color:#4ea8de; margin:10px 0">
        ${game.price === 0 ? "FREE" : "Rp " + game.price.toLocaleString('id-ID')}
      </p>
      ${owned ? '<div style="background:#2ecc71; color:white; padding:5px 10px; border-radius:4px; text-align:center; font-size:12px; font-weight:bold;">Owned</div>' : ''}
    `;

    card.appendChild(img);
    card.appendChild(contentDiv);
    grid.appendChild(card);
  });
}

function handleAddToCart(id) {
  if (typeof addToCart === "function") {
    if (addToCart(id)) alert("Berhasil ditambah ke keranjang!");
  }
}

function handleAddToWishlist(id) {
  if (typeof addToWishlist === "function") {
    if (addToWishlist(id)) alert("Berhasil ditambah ke wishlist!");
  }
}

// Fitur Search
let searchHandler = null;

function initSearchListener() {
  const searchInput = document.getElementById("searchInput");

  if (searchInput && !searchHandler) {
    searchHandler = (e) => {
      const q = e.target.value.toLowerCase();
      const filtered = games.filter(g =>
        g.name.toLowerCase().includes(q) ||
        g.category.toLowerCase().includes(q)
      );
      renderStore(filtered);
    };

    searchInput.addEventListener("input", searchHandler);
  }
}

/* ========= FILTERS & SORT ========= */
function filterGames(filters) {
  let filtered = [...games];

  // Price range filter
  if (filters.priceMin !== undefined) {
    filtered = filtered.filter(g => g.price >= filters.priceMin);
  }
  if (filters.priceMax !== undefined) {
    filtered = filtered.filter(g => g.price <= filters.priceMax);
  }

  // Free games only filter
  if (filters.freeOnly) {
    filtered = filtered.filter(g => g.price === 0);
  }

  // Sort
  if (filters.sortBy) {
    switch (filters.sortBy) {
      case 'price-asc':
        filtered.sort((a, b) => a.price - b.price);
        break;
      case 'price-desc':
        filtered.sort((a, b) => b.price - a.price);
        break;
      case 'rating-desc':
        filtered.sort((a, b) => b.rating - a.rating);
        break;
      case 'name-asc':
        filtered.sort((a, b) => a.name.localeCompare(b.name));
        break;
      default:
        // Default: keep original order
        break;
    }
  }

  return filtered;
}

function applyFilters() {
  const priceMin = parseInt(document.getElementById("priceMin")?.value) || 0;
  const priceMax = parseInt(document.getElementById("priceMax")?.value) || 999999;
  const freeOnly = document.getElementById("freeOnly")?.checked || false;
  const sortBy = document.getElementById("sortBy")?.value || 'default';

  const filters = {
    priceMin,
    priceMax,
    freeOnly,
    sortBy
  };

  const filtered = filterGames(filters);
  renderStore(filtered);

  // Show count
  const grid = document.getElementById("gameGrid");
  if (grid && filtered.length === 0) {
    grid.innerHTML = '<p style="text-align: center; grid-column: 1/-1; color: #999;">Tidak ada game yang sesuai dengan filter.</p>';
  }
}

function resetFilters() {
  // Reset all filter inputs
  if (document.getElementById("priceMin")) document.getElementById("priceMin").value = "0";
  if (document.getElementById("priceMax")) document.getElementById("priceMax").value = "100000";
  if (document.getElementById("freeOnly")) document.getElementById("freeOnly").checked = false;
  if (document.getElementById("sortBy")) document.getElementById("sortBy").value = "default";

  // Re-render all games
  renderStore(games);
}

document.addEventListener("DOMContentLoaded", () => {
  initSearchListener();
  renderStore(games);
});

/* ========= QUICK ACTION MODAL ========= */
function showQuickActionModal(gameId) {
  const game = games.find(g => g.id === gameId);
  if (!game) return;

  const owned = isOwned(gameId);

  // Create modal overlay
  const modal = document.createElement("div");
  modal.id = "quickActionModal";
  modal.style.cssText = `
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background: rgba(0, 0, 0, 0.8);
    backdrop-filter: blur(5px);
    display: flex;
    align-items: center;
    justify-content: center;
    z-index: 9999;
  `;

  // Modal content
  const modalContent = document.createElement("div");
  modalContent.style.cssText = `
    background: #2b2d42;
    border-radius: 15px;
    padding: 30px;
    max-width: 500px;
    width: 90%;
    box-shadow: 0 10px 40px rgba(0, 0, 0, 0.5);
    position: relative;
  `;

  modalContent.innerHTML = `
    <div style="text-align: center; margin-bottom: 20px;">
      <img src="${escapeHTML(game.image)}" alt="${escapeHTML(game.name)}" 
           style="width: 100%; max-height: 200px; object-fit: cover; border-radius: 10px; margin-bottom: 15px;">
      <h2 style="margin: 10px 0; color: #fff;">${escapeHTML(game.name)}</h2>
      <p style="color: #aaa; margin: 5px 0;">${escapeHTML(game.category)} | Rating: ${game.rating}</p>
      <p style="font-size: 24px; font-weight: bold; color: #4ea8de; margin: 10px 0;">
        ${game.price === 0 ? "FREE" : "Rp " + game.price.toLocaleString('id-ID')}
      </p>
    </div>

    <div style="display: grid; gap: 10px; margin-bottom: 15px;">
      ${owned
      ? `<button disabled style="
            padding: 15px;
            background: #444;
            color: #888;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
          ">Already Owned</button>`
      : `
          <button onclick="quickBuyNow(${gameId})" style="
            padding: 15px;
            background: linear-gradient(135deg, #2ecc71, #27ae60);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: 0.3s;
          " onmouseover="this.style.transform='translateY(-2px)'" onmouseout="this.style.transform='translateY(0)'">
            Buy Now
          </button>
          <button onclick="quickAddToCart(${gameId})" style="
            padding: 15px;
            background: #4ea8de;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: 0.3s;
          " onmouseover="this.style.transform='translateY(-2px)'" onmouseout="this.style.transform='translateY(0)'">
            Add to Cart
          </button>
        `
    }
      <button onclick="quickAddToWishlist(${gameId})" style="
        padding: 15px;
        background: rgba(231, 76, 60, 0.8);
        color: white;
        border: none;
        border-radius: 8px;
        font-size: 16px;
        font-weight: bold;
        cursor: pointer;
        transition: 0.3s;
      " onmouseover="this.style.transform='translateY(-2px)'" onmouseout="this.style.transform='translateY(0)'">
        Add to Wishlist
      </button>
    </div>

    <button onclick="closeQuickActionModal()" style="
      width: 100%;
      padding: 12px;
      background: rgba(255, 255, 255, 0.1);
      color: #fff;
      border: 1px solid rgba(255, 255, 255, 0.2);
      border-radius: 8px;
      font-size: 14px;
      font-weight: bold;
      cursor: pointer;
    ">Close</button>
  `;

  modal.appendChild(modalContent);
  document.body.appendChild(modal);

  // Click outside to close
  modal.addEventListener("click", (e) => {
    if (e.target === modal) {
      closeQuickActionModal();
    }
  });
}

function closeQuickActionModal() {
  const modal = document.getElementById("quickActionModal");
  if (modal) {
    modal.remove();
  }
}

function quickBuyNow(gameId) {
  const user = getCurrentUser();
  if (!user) {
    alert("Silakan login terlebih dahulu!");
    window.location.href = "auth.html";
    return;
  }

  if (isOwned(gameId)) {
    alert("Anda sudah memiliki game ini!");
    closeQuickActionModal();
    return;
  }

  // Add to browsing history
  if (typeof addToBrowsingHistory === "function") {
    addToBrowsingHistory(gameId);
  }

  // Add to cart and redirect to checkout
  if (typeof addToCart === "function") {
    addToCart(gameId);
    closeQuickActionModal();
    window.location.href = "cart.html";
  }
}

function quickAddToCart(gameId) {
  const user = getCurrentUser();
  if (!user) {
    alert("Silakan login terlebih dahulu!");
    window.location.href = "auth.html";
    return;
  }

  if (typeof addToCart === "function") {
    if (addToCart(gameId)) {
      alert("Berhasil ditambahkan ke keranjang!");
      closeQuickActionModal();
    }
  }
}

function quickAddToWishlist(gameId) {
  const user = getCurrentUser();
  if (!user) {
    alert("Silakan login terlebih dahulu!");
    window.location.href = "auth.html";
    return;
  }

  if (typeof addToWishlist === "function") {
    if (addToWishlist(gameId)) {
      alert("Berhasil ditambahkan ke wishlist!");
      closeQuickActionModal();
    }
  }
}

/* ========= XSS PROTECTION ========= */
function escapeHTML(text) {
  const div = document.createElement('div');
  div.textContent = text;
  return div.innerHTML;
}