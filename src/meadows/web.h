#ifndef WEB_H
#define WEB_H
#include <pgmspace.h>

const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="id">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Solder Station PRO</title>
<style>
body{
  margin:0;
  font-family:Segoe UI,sans-serif;
  background:linear-gradient(135deg,#0f172a,#1e293b,#020617);
  color:white;
}
.topbar{
  padding:12px;
  background:rgba(0,0,0,.3);
  backdrop-filter:blur(10px);
  border-bottom:1px solid rgba(255,255,255,.08);
  font-weight:bold;
}
.menuBtn{
  float:right;
  font-size:26px;
  cursor:pointer;
}
.menuBox{
  position:fixed;
  top:50px;
  right:10px;
  width:220px;
  background:rgba(255,255,255,.05);
  backdrop-filter:blur(15px);
  border:1px solid rgba(255,255,255,.1);
  border-radius:12px;
  display:none;
  z-index:999;
}
.menuBox div{
  padding:12px;
  border-bottom:1px solid rgba(255,255,255,.08);
  cursor:pointer;
}
.menuBox div:hover{
  background:rgba(255,255,255,.1);
}
.card{
  margin:10px;
  padding:15px;
  border-radius:16px;
  background:rgba(255,255,255,.05);
  backdrop-filter:blur(15px);
  border:1px solid rgba(255,255,255,.08);
}
button{
  padding:10px;
  margin:4px;
  border:none;
  border-radius:10px;
  background:linear-gradient(90deg,#06b6d4,#2563eb);
  color:white;
  font-weight:bold;
}
input[type=range],select,input[type=file]{
  width:100%;
}
.big{
  font-size:42px;
  text-align:center;
}
.statusBar{
  display:flex;
  justify-content:space-between;
  font-size:12px;
  opacity:.8;
}
.sleepBtn{
  background:#1e293b;
  color:white;
  border:1px solid rgba(255,255,255,.1);
}
.sleepBtn.active{
  background:linear-gradient(90deg,#22c55e,#06b6d4);
  color:black;
}
.pwrBtn{
  width:100%;
  padding:14px;
  font-size:16px;
  margin-top:8px;
}
.pwrBtn.on{
  background:linear-gradient(90deg,#22c55e,#16a34a);
}
.pwrBtn.off{
  background:linear-gradient(90deg,#ef4444,#b91c1c);
}
progress{
  width:100%;
}
</style>
</head>
<body>

<div class="topbar">
🔥 SOLDER / HOTAIR STATION 🔥
<span class="menuBtn" onclick="toggleMenu()">☰</span>
</div>

<div id="menuBox" class="menuBox">
  <div onclick="show('dash')">🔥 Solder Station</div>
  <div onclick="show('hotair')">💨 HotAir Station</div>
  <div onclick="show('pid')">⚙ PID</div>
  <div onclick="show('tipcfg')">🔧 Tip Settings</div>
  <div onclick="show('templimit')">🌡 Temp Limit</div>
  <div onclick="show('sleep')">😴 Sleep</div>
  <div onclick="show('ota')">⬆ OTA Update</div>
</div>

<!-- ================= SOLDER STATION ================= -->
<div id="dash">

<div class="card statusBar">
  <div>📶 WiFi: <span id="wifi">--</span></div>
  <div>🔧 Tip: <span id="tip">--</span></div>
  <div>⚡ Mode: <span id="mode">PID</span></div>
</div>

<div class="card">
  <div style="display:flex;justify-content:space-between;align-items:center">
    <div style="font-size:15px;line-height:1.9">
      <div>Temp Limit : <span id="maxTempVal">--</span>°C</div>
      <div>Temp Set : <span id="set">--</span>°C</div>
      <div>PWM : <span id="pwm">--</span>%</div>
    </div>
    <div class="big" id="temp" style="font-size:52px;font-weight:bold">--°C</div>
  </div>
</div>

<div class="card">
<h3>Set Temperature</h3>
<input type="range" id="setSlider" min="100" max="450" value="320"
  oninput="setTemp(this.value)">
<div style="text-align:center">Set: <span id="setVal">320</span>°C</div>
</div>

<div class="card">
<h3>PWM Control</h3>
<input type="range" id="pwmSlider" min="0" max="100" value="0"
  oninput="setPWM(this.value)">
<div style="text-align:center">PWM: <span id="pwmVal">0</span> %</div>
<button onclick="setPWM(15)">15</button>
<button onclick="setPWM(30)">30</button>
<button onclick="setPWM(50)">50</button>
<button onclick="setPWM(70)">70</button>
<button onclick="setPWM(90)">90</button>
<button onclick="boost()">BOOST</button>
</div>

</div>

<!-- ================= HOT AIR STATION ================= -->
<div id="hotair" style="display:none">

<div class="card statusBar">
  <div>📶 WiFi: <span id="wifiHa">--</span></div>
  <div>💨 AC: <span id="acHa">--</span></div>
  <div>⚡ Mode: <span id="modeHa">OFF</span></div>
</div>

<div class="card">
  <div style="display:flex;justify-content:space-between;align-items:center">
    <div style="font-size:15px;line-height:1.9">
      <div>Temp Max : <span id="maxTempHa">550</span>°C</div>
      <div>Temp Set : <span id="setHa">300</span>°C</div>
      <div>Power : <span id="pwrHa">0</span></div>
      <div>Fan : <span id="fanHa">0</span></div>
    </div>
    <div class="big" id="tempHa" style="font-size:52px;font-weight:bold">--°C</div>
  </div>
</div>

<div class="card">
<button id="pwrBtnHa" class="pwrBtn off" onclick="toggleHaPower()">POWER OFF</button>
</div>

<div class="card">
<h3>Set Temperature</h3>
<!-- TEMP_MIN=100, TEMP_MAX_HOTAIR=550, DEFAULT=300 -->
<input type="range" id="setSliderHa" min="100" max="550" value="300"
  oninput="setTempHa(this.value)">
<div style="text-align:center">Set: <span id="setValHa">300</span>°C</div>
</div>

<div class="card">
<h3>Air Flow / Fan</h3>
<!-- MAX_FAN_PWM = 255 -->
<input type="range" id="airSlider" min="0" max="255" value="128"
  oninput="setAir(this.value)">
<div style="text-align:center">Fan: <span id="airVal">128</span></div>
<button onclick="setAir(50)">50</button>
<button onclick="setAir(100)">100</button>
<button onclick="setAir(150)">150</button>
<button onclick="setAir(200)">200</button>
<button onclick="setAir(255)">255</button>
</div>

</div>

<!-- TEMP LIMIT -->
<div id="templimit" style="display:none">
<div class="card">
<h3>Temperature Limit (Solder)</h3>
<input type="range" min="100" max="450" value="320" id="tempLock"
  oninput="setTempLock(this.value)">
<div style="text-align:center">Max: <span id="maxTempVal2">320</span>°C</div>
</div>
</div>

<!-- PID -->
<div id="pid" style="display:none">
<div class="card">
<h3>PID Tuning</h3>
Kp <input type="range" id="kp" min="0" max="40" step="0.1"><br>
Ki <input type="range" id="ki" min="0" max="2" step="0.01"><br>
Kd <input type="range" id="kd" min="0" max="80" step="0.1"><br>
<button onclick="sendPID()">SAVE PID</button>
</div>
</div>

<!-- TIP CONFIG -->
<div id="tipcfg" style="display:none">
<div class="card">
<h3>Tip Profile</h3>
<select id="tipSelect" onchange="setTip(this.value)">
  <option value="T12">T12</option>
  <option value="C210">C210</option>
  <option value="AUTO">AUTO</option>
</select>
<div style="margin-top:12px">
Current: <b id="tipCurrent">T12 READY</b>
</div>
</div>
</div>

<!-- SLEEP -->
<div id="sleep" style="display:none">
<div class="card">
<h3>Sleep Timer</h3>
<button class="sleepBtn" id="s10" onclick="setSleepUI(10)">10s</button>
<button class="sleepBtn" id="s15" onclick="setSleepUI(15)">15s</button>
<button class="sleepBtn" id="s20" onclick="setSleepUI(20)">20s</button>
<button class="sleepBtn" id="s30" onclick="setSleepUI(30)">30s</button>
<div id="sleepText">SLEEP: OFF</div>
</div>
</div>

<!-- OTA -->
<div id="ota" style="display:none">
<div class="card">
<h3>OTA Firmware Update</h3>
<form id="uploadForm" method="POST" action="/update" enctype="multipart/form-data">
<input type="file" id="firmware" name="update" accept=".bin">
<button type="submit">UPLOAD</button>
</form>
</div>
<div class="card">
<h3>OTA Progress</h3>
<progress id="otabar" value="0" max="100"></progress>
<div id="otatext">Idle</div>
</div>
</div>

<script>
let ws;
let maxTemp = 320;
let haPowerOn = false;

function toggleMenu(){
  menuBox.style.display =
  (menuBox.style.display=="block") ? "none" : "block";
}

function show(id){
  ["dash","hotair","pid","tipcfg","templimit","sleep","ota"].forEach(n=>{
    let el = document.getElementById(n);
    if(el) el.style.display = "none";
  });
  document.getElementById(id).style.display = "block";
  menuBox.style.display = "none";
}

function connect(){
  ws = new WebSocket("ws://" + location.hostname + ":81");

  ws.onopen = ()=>{
    console.log("WS CONNECTED");
  };

  ws.onmessage = (e)=>{
    let d = JSON.parse(e.data);

    // ----- Solder -----
    if (d.temp !== undefined) temp.innerHTML = d.temp + "°C";
    if (d.set !== undefined) {
      set.innerHTML = d.set;
      setSlider.value = d.set;
      setVal.innerHTML = d.set;
    }
    if (d.pwm !== undefined) {
      pwm.innerHTML = d.pwm;
      pwmSlider.value = d.pwm;
      pwmVal.innerHTML = d.pwm;
    }
    if (d.wifi !== undefined) {
      wifi.innerHTML = d.wifi ? "ON" : "OFF";
      wifiHa.innerHTML = d.wifi ? "ON" : "OFF";
    }
    if (d.tip !== undefined) {
      tip.innerHTML = d.tip;
      tipCurrent.innerHTML = d.tip + " READY";
      tipSelect.value = d.tip;
    }
    if (d.state !== undefined) mode.innerHTML = d.state;

    if (d.maxTemp !== undefined) {
      maxTemp = d.maxTemp;
      maxTempVal.innerHTML = d.maxTemp;
      maxTempVal2.innerHTML = d.maxTemp;
      setSlider.max = d.maxTemp;
      tempLock.max = d.maxTemp;
    }

    if (d.kp !== undefined) kp.value = d.kp;
    if (d.ki !== undefined) ki.value = d.ki;
    if (d.kd !== undefined) kd.value = d.kd;

    // ----- Hot Air -----
    if (d.haTemp !== undefined) tempHa.innerHTML = d.haTemp + "°C";
    if (d.haSet !== undefined) {
      setHa.innerHTML = d.haSet;
      setSliderHa.value = d.haSet;
      setValHa.innerHTML = d.haSet;
    }
    if (d.haPower !== undefined) {
      pwrHa.innerHTML = d.haPower;
    }
    if (d.haFan !== undefined) {
      fanHa.innerHTML = d.haFan;
      airSlider.value = d.haFan;
      airVal.innerHTML = d.haFan;
    }
    if (d.haMode !== undefined) {
      modeHa.innerHTML = d.haMode;
      haPowerOn = (d.haMode === "ON" || d.haMode === "FIXED");
      updatePwrBtn();
    }
    if (d.haAC !== undefined) {
      acHa.innerHTML = d.haAC ? "OK" : "--";
    }
    if (d.haMaxTemp !== undefined) {
      maxTempHa.innerHTML = d.haMaxTemp;
      setSliderHa.max = d.haMaxTemp;
    }
  };
}

connect();

function safeSend(msg){
  if(ws && ws.readyState === WebSocket.OPEN){
    ws.send(msg);
  }
}

function updatePwrBtn(){
  let btn = document.getElementById("pwrBtnHa");
  if(haPowerOn){
    btn.textContent = "POWER ON";
    btn.className = "pwrBtn on";
  } else {
    btn.textContent = "POWER OFF";
    btn.className = "pwrBtn off";
  }
}

/* ================= SOLDER ================= */

function setTemp(v){
  v = parseInt(v);
  setSlider.value = v;
  setVal.innerHTML = v;
  safeSend("SET:" + v);
}

function setPWM(v){
  v = parseInt(v);
  pwmSlider.value = v;
  pwmVal.innerHTML = v;
  safeSend("PWM:" + v);
}

function boost(){
  safeSend("BOOST");
}

function setTempLock(v){
  v = parseInt(v);
  maxTemp = v;
  maxTempVal.innerHTML = v;
  maxTempVal2.innerHTML = v;
  safeSend("MAXTEMP:" + v);
}

function sendPID(){
  safeSend("PID:" + parseFloat(kp.value) + "," + parseFloat(ki.value) + "," + parseFloat(kd.value));
}

function setTip(name){
  tipCurrent.innerHTML = name + " READY";
  safeSend("TIP:" + name);
}

function setSleepUI(sec){
  document.querySelectorAll(".sleepBtn").forEach(btn=>{
    btn.classList.remove("active");
  });
  let btn = document.getElementById("s"+sec);
  if(btn) btn.classList.add("active");
  sleepText.innerHTML = "SLEEP ACTIVE : " + sec + "s";
  safeSend("SLEEPTIME:" + sec);
}

/* ================= HOT AIR ================= */
/* Config: TEMP_MIN=100, TEMP_MAX_HOTAIR=550, DEFAULT=300
   MAX_PWM_HOTAIR=255, MAX_FAN_PWM=255 */

function setTempHa(v){
  v = parseInt(v);
  setSliderHa.value = v;
  setValHa.innerHTML = v;
  setHa.innerHTML = v;
  safeSend("HASET:" + v);
}

function setAir(v){
  v = parseInt(v);
  airSlider.value = v;
  airVal.innerHTML = v;
  fanHa.innerHTML = v;
  safeSend("AIR:" + v);
}

function toggleHaPower(){
  haPowerOn = !haPowerOn;
  updatePwrBtn();
  safeSend("HAPWR:" + (haPowerOn ? "1" : "0"));
}

</script>
</body>
</html>

)rawliteral";

#endif