let mode = "login"; // login | register
let role = "user"; // user | publisher

function switchMode(){
  mode = mode === "login" ? "register" : "login";

  document.getElementById("title").innerText = mode === "login" ? "Login" : "Register";
  document.getElementById("mainBtn").innerText = mode === "login" ? "Login" : "Register";

  // Tampilkan tombol switch role hanya saat register
  const roleSwitch = document.getElementById("roleSwitch");
  roleSwitch.style.display = mode === "register" ? "inline" : "none";

  // Reset form
  if (mode === "login") {
    role = "user";
    document.getElementById("publisherForm").style.display = "none";
    roleSwitch.innerText = "Daftar sebagai Publisher";
  }
}

function switchRole() {
  if (mode !== "register") return;

  role = role === "user" ? "publisher" : "user";
  const publisherForm = document.getElementById("publisherForm");
  const roleSwitch = document.getElementById("roleSwitch");

  if (role === "publisher") {
    publisherForm.style.display = "block";
    roleSwitch.innerText = "Daftar sebagai User";
  } else {
    publisherForm.style.display = "none";
    roleSwitch.innerText = "Daftar sebagai Publisher";
  }
}

function submitForm(){
  const username = document.getElementById("username").value.trim();
  const password = document.getElementById("password").value.trim();

  if(!username || !password){
    alert("Username dan password wajib diisi");
    return;
  }

  if(mode === "login"){
    if(login(username, password)){
      window.location.href = "index.html";
    }
  } else {
    // Register
    if (role === "publisher") {
      const studioName = document.getElementById("studioName").value.trim();
      const country = document.getElementById("country").value.trim();
      const yearFounded = document.getElementById("yearFounded").value.trim();

      if (!studioName || !country || !yearFounded) {
        alert("Semua field Publisher wajib diisi");
        return;
      }

      if (registerPublisher(username, password, studioName, country, yearFounded)) {
        alert("Registrasi Publisher berhasil! Silakan login.");
        switchMode();
      }
    } else {
      if(register(username, password)){
        alert("Registrasi berhasil, silakan login");
        switchMode();
      }
    }
  }
}

function forgotPassword(){
  const username = prompt("Masukkan username Anda");
  if(!username) return;

  const users = JSON.parse(localStorage.getItem("users")) || [];
  const user = users.find(u => u.username === username);

  if(!user){
    alert("Username tidak ditemukan");
    return;
  }

  const newPass = prompt("Masukkan password baru");
  if(!newPass){
    alert("Password tidak boleh kosong");
    return;
  }

  user.password = newPass;
  localStorage.setItem("users", JSON.stringify(users));
  alert("Password berhasil diubah. Silakan login.");
}