
static const char html_header[] PROGMEM =
  "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1, user-scalable=0\">"
  "<title>FLORA - Wi-Fi VFD Clock</title><style>html,body{margin:0;padding:0;font-size:16px;background:#333;}body,*{box-sizing:border-box;font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,\"Helvetica Neue\",Arial,sans-serif;}a{color:inherit;text-decoration:underline;}.wrapper{padding:30px 0;}.container{margin:auto;padding:40px;max-width:600px;color:#fff;background:#000;box-shadow:0 0 100px rgba(0,0,0,.5);border: 10px solid #444;border-radius:50px;}.row{margin-bottom:15px;}.flexrow{display:flex;justify-items:stretch;align-items:flex-start;flex-wrap:wrap;margin-left:-10px;}.col{flex: 1;padding-left:10px;}h1{margin:0 0 10px 0;font-family:Arial,sans-serif;font-weight:300;font-size:2rem;}h1 + p{margin-bottom:30px;}h2{margin:30px 0 0 0;font-family:Arial,sans-serif;font-weight:300;font-size:1.5rem;}h3{font-family:Arial,sans-serif;font-weight:300;font-size:1.2rem;margin: 25px 0 10px 0;}div+h3{margin-top:10px;}p{font-size:.85rem;margin:0 0 20px 0;color:rgba(255,255,255,.7);}label{display:block;width:100%;margin-bottom:5px;}label+p{margin-bottom:5px;}input[type=\"text\"],input[type=\"number\"],input[type=\"password\"],select{display:inline-block;width:100%;height:42px;line-height:38px;padding:0 20px;color:#fff;border:2px solid #666;background:none;border-radius:5px;transition:.15s;box-shadow:none;outline:none;}input[type=\"text\"]:hover,input[type=\"number\"]:hover,input[type=\"password\"]:hover,select:hover{border-color:#69b6ac;}input[type=\"text\"]:focus,input[type=\"password\"]:focus,select:focus{border-color:#a5fff3;}option{color:#000;}button{display:block;width:100%;padding:10px 20px;font-size:1rem;font-weight:700;text-transform:uppercase;background:#43ffe5;border:0;border-radius:5px;cursor:pointer;transition:.15s;outline:none;}button:hover{background:#a5fff3;}.github{padding:15px;text-align:center;}.github a{color:#43ffe5;transition:.15s;}.github a:hover{color:#a5fff3;}.github p{margin:0;}.mac{display:inline-block;margin-top:8px;padding:2px 5px;color:#fff;background:#444;border-radius:3px;}</style><style media=\"all and (max-width:520px)\">.wrapper{padding:0;}.container{padding:25px 15px;border:0;border-radius:0;}.col{flex: auto;width:50%;}</style></head><body><div class=\"wrapper\">"
  "<div class=\"container\">";

static const char html_footer[] PROGMEM =
  "</div>"
  "<div class=\"github\"><p>"
  "{{fw_name}} {{fw_version}}"
  ", check out <a href=\"https://github.com/mcer12/Flora-ESP8266\" target=\"_blank\"><strong>FLORA</strong> on GitHub</a></p> </div>"
  "</div>"
  "<script>function toggleVisibility(eventsender, idOfObjectToToggle){var myNewState = \"none\";if (eventsender.checked === true){myNewState = \"block\";}document.getElementById(idOfObjectToToggle).style.display = myNewState;}toggleVisibility(document.getElementById('dst_enable'), 'dst_wrapper');toggleVisibility(document.getElementById('bal_enable'), 'bal_wrapper');</script>"
  "</body> </html>";

/** Is this an IP? */
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (deviceMode == CONFIG_MODE && !isIp(server.hostHeader()) && server.hostHeader() != String(dns_name) + ".local") {
    Serial.print("[SERV] Request to ");
    Serial.print(server.hostHeader());
    Serial.println(" redirected to captive portal");
    server.sendHeader("Location", "http://" + String(dns_name) + ".local"/* + toStringIp(server.client().localIP())*/, true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

bool startMDNS() {
  int tryCount = 0;
  if (!MDNS.begin(dns_name, WiFi.softAPIP())) {
    Serial.println("[SERV] MDNS responder did not setup");
    while (tryCount < 10) {
      tryCount++;
      delay(500);
    }
    return false;
  }
  return true;
}

void startConfigPortal() {
  configStartMillis = millis(); // start counter

  updateColonColor(colorConfigMode);
  strip_show();

  String ap_name = AP_NAME + macLastThreeSegments(mac);
  IPAddress ap_ip(8, 8, 8, 8);

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_ip, ap_ip, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ap_name.c_str());
  delay(100);
  Serial.print("[SERV] IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", ap_ip);

  startMDNS();
  startServer();

  Serial.println("[SERV] Captive portal started");

  unsigned int lastTest = 0;

  while (deviceMode == CONFIG_MODE) { // BLOCKING INFINITE LOOP

    time_t remainingSeconds = (CONFIG_TIMEOUT - (millis() - configStartMillis)) / 1000;

    if (millis() - configStartMillis > CONFIG_TIMEOUT) {
      WiFi.softAPdisconnect(true);
      //WiFi.mode(WIFI_STA);
      deviceMode = CONNECTION_FAIL;
      return;
    }

    if (millis() > lastTest + 100) {
      int splitTime[] = {

#ifndef CLOCK_VERSION_IV22
        (hour(remainingSeconds) / 10) % 10,
        hour(remainingSeconds) % 10,
#endif
        (minute(remainingSeconds) / 10) % 10,
        minute(remainingSeconds) % 10,
        (second(remainingSeconds) / 10) % 10,
        second(remainingSeconds) % 10,
      };

      for (int i = 0; i < registersCount; i++) {
#ifndef CLOCK_VERSION_IV22
        if (i < 4 && splitTime[i] == 0 && registersCount > 4) {
          blankDigit(i);
          continue;
        }
#endif
        setDigit(i, splitTime[i]);
      }
      lastTest = millis();
    }

    MDNS.update();
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();
    delay(10);
  }
}

bool startServer() {
  httpUpdateServer.setup(&server, update_path, update_username, update_password);
  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  //server.on("/json", handleJson); // show raw json at given url path

  // diyHue
  server.on("/detect", handleDiyHueDetect);
  server.on("/set", handleDiyHueSet);
  //server.on("/switch", handleDiyHueSwitch);
  server.on("/get", handleDiyHueGet);

  server.onNotFound(handleNotFound);
  server.begin();
}

void handleDiyHueDetect() {
  server.send(200, "text/plain", "{\"hue\": \"bulb\",\"lights\": " + String(1) + ",\"name\": \"" + AP_NAME + macLastThreeSegments(mac) +  "\",\"modelid\": \"LWB010\",\"mac\": \"" + String(mac[5], HEX) + ":"  + String(mac[4], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[0], HEX) + "\"}");
}

void handleDiyHueGet() {
  int diyhueBri;
  if (bri == 0) diyhueBri = 0;
  if (bri == 1) diyhueBri = 128;
  if (bri == 2) diyhueBri = 255;
  String power_status = isPoweredOn ? "true" : "false";
  server.send(200, "text/plain", "{\"on\": " + power_status + ", \"bri\": " + String(diyhueBri) + "}");
}

void handleDiyHueSet() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "on") {
      if (server.arg(i) == "True" || server.arg(i) == "true") {
        isPoweredOn = true;
      }
      else {
        isPoweredOn = false;
      }
    }
    else if (server.argName(i) == "bri" || server.argName(i) == "bri_inc") {
      isPoweredOn = true;
      if (server.arg(i).toInt() != 0) {
        if (server.arg(i).toInt() < 85) bri = 0;
        else if (server.arg(i).toInt() < 170) bri = 1;
        else bri = 2;
      }
    }
    else if (server.argName(i) == "alert" && server.arg(i) == "select") {
      if (isPoweredOn) {
        bri = 0;
      } else {
        bri = 2;
      }
    }
  }

  json["bri"] = bri;

  int diyhueBri;
  if (bri == 0) diyhueBri = 0;
  if (bri == 1) diyhueBri = 128;
  if (bri == 2) diyhueBri = 255;
  String power_status = isPoweredOn ? "true" : "false";
  server.send(200, "text/plain", "OK, bri:" + String(diyhueBri) + ", state:" + power_status);
}


void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }

  String html = FPSTR(html_header);
  html += "<h1>404 - Page not found</h1>";
  html += "<h2>\"The right man in the wrong place can make all the difference in the world.\"</h2>";
  for (uint8_t i = 0; i < server.args(); i++) {
    html += "<p>";
    html += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
    html += "</p>";
  }
  html += FPSTR(html_footer);

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  server.send(404, "text/html", html);
}
/*
  // Use only for testing => reveals wifi password!
  void handleJson() {
  String html;
  serializeJson(json, html);

  server.send(200, "text/json", html);

  }
*/
void handleRoot() {
  unsigned long serverProcessingTime = millis();

  if (server.args()) {

    if (server.hasArg("is_form")) {
      updateColonColor(colorConfigSave);
      strip_show();
    }

    if (server.hasArg("ssid")) {
      json["ssid"] = server.arg("ssid");
    }
    if (server.hasArg("pass") && server.arg("pass") == "placeholder") {
      Serial.println("password is a placeholder, skipped!");
    }
    if (server.hasArg("pass") && server.arg("pass") != "placeholder") {
      json["pass"] = server.arg("pass");
    }
    /*
      if (server.hasArg("ip")) {
      json["ip"] = server.arg("ip");
      }
      if (server.hasArg("gw")) {
      json["gw"] = server.arg("gw");
      }
      if (server.hasArg("sn")) {
      json["sn"] = server.arg("sn");
      }
    */
    if (server.hasArg("t_format")) {
      json["t_format"] = server.arg("t_format");
    }
    if (server.hasArg("zero")) {
      json["zero"] = server.arg("zero");
    }
    if (server.hasArg("on")) {
      isPoweredOn = server.arg("on").toInt();
    }
    if (server.hasArg("bri")) {
      json["bri"] = server.arg("bri");
      bri = json["bri"].as<int>();
      setupPhaseShift();
    }
    if (server.hasArg("fade")) {
      json["fade"] = server.arg("fade");
      crossFadeTime = json["fade"].as<int>();
      //setupCrossFade();
    }
    if (server.hasArg("colon")) {
      json["colon"] = server.arg("colon");
    }
    if (server.hasArg("nmode")) {
      json["nmode"] = server.arg("nmode");
    }
    if (server.hasArg("cathode")) {
      json["cathode"] = server.arg("cathode");
    }
    if (server.hasArg("rst_cycle")) {
      json["rst_cycle"] = server.arg("rst_cycle");
    }
    if (server.hasArg("rst_ip")) {
      json["rst_ip"] = server.arg("rst_ip");
    }
    if (server.hasArg("bal_enable")) {
      json["bal_enable"] = server.arg("bal_enable");
    } else {
      json["bal_enable"] = 0;
    }
    for (int i = 0; i < registersCount; i++) {
      if (server.hasArg("bal_h_" + String(i))) {
        json["bal"]["high"][i] = server.arg("bal_h_" + String(i));
      }
      if (server.hasArg("bal_l_" + String(i))) {
        json["bal"]["low"][i] = server.arg("bal_l_" + String(i));
      }
    }
    if (server.hasArg("ntp")) {
      json["ntp"] = server.arg("ntp");
    }
    if (server.hasArg("std_offset")) {
      json["std_offset"] = server.arg("std_offset");
    }
    if (server.hasArg("dst_enable")) {
      json["dst_enable"] = server.arg("dst_enable");
    } else {
      json["dst_enable"] = 0;
    }
    if (server.hasArg("std_week")) {
      json["std_week"] = server.arg("std_week");
    }
    if (server.hasArg("std_day")) {
      json["std_day"] = server.arg("std_day");
    }
    if (server.hasArg("std_month")) {
      json["std_month"] = server.arg("std_month");
    }
    if (server.hasArg("std_hour")) {
      json["std_hour"] = server.arg("std_hour");
    }
    if (server.hasArg("dst_offset")) {
      json["dst_offset"] = server.arg("dst_offset");
    }
    if (server.hasArg("dst_week")) {
      json["dst_week"] = server.arg("dst_week");
    }
    if (server.hasArg("dst_day")) {
      json["dst_day"] = server.arg("dst_day");
    }
    if (server.hasArg("dst_month")) {
      json["dst_month"] = server.arg("dst_month");
    }
    if (server.hasArg("dst_hour")) {
      json["dst_hour"] = server.arg("dst_hour");
    }
    saveConfig();

  }



  String html = "";

  if (!server.args() || (server.args() && server.hasArg("is_form"))) {
    html += FPSTR(html_header);
    html += "<form method=\"post\" action=\"/\">";
    html += "<h1>FLORA - Wi-Fi VFD Clock</h1> <p></p>";

    if (deviceMode == CONFIG_MODE) {
      html += "<p>Config AP will automatically shut down after 5 minutes.</p> <p>After you save the settings and connect to wifi, you can open the config portal again using Clock's local IP address.</p>";
    }

    html += "<h2>Network settings</h2> <p>Select your network settings here.</p> <div class=\"row\"> <label for=\"ssid\">WiFi SSID</label> <input type=\"text\" id=\"ssid\" name=\"ssid\" value=\"";
    html += json["ssid"].as<const char*>();
    html += "\"> </div> <div class=\"row\"> <label for=\"pass\">WiFi Password</label> <input type=\"password\" id=\"pass\" name=\"pass\" value=\"";
    const char* pass = json["pass"].as<const char*>();
    if (pass != NULL && pass[0] != '\0') {
      html += "placeholder";
    }
    /*
        html += "\"> </div> <h2>Static IP settings (optional)</h2> <p>Optional settings for static IP, in some cases this might speed up response time. All 3 need to be set and IP should be reserved in router's DHCP settings.";
        html += "<br>MAC address: <span class=\"mac\">";
        html += macToStr(mac);
        html += "</span></p>";
        html += "<div class=\"row\"> <label for=\"ip\">IP Address (optional):</label> <input type=\"text\" id=\"ip\" name=\"ip\" value=\"";
        html += json["ip"].as<const char*>();
        html += "\"> </div> <div class=\"row\"> <label for=\"gw\">Gateway IP (optional):</label> <input type=\"text\" id=\"gw\" name=\"gw\" value=\"";
        html += json["gw"].as<const char*>();
        html += "\"> </div> <div class=\"row\"> <label for=\"sn\">Subnet mask (optional):</label> <input type=\"text\" id=\"sn\" name=\"sn\" value=\"";
        html += json["sn"].as<const char*>();
    */
    html += "\"> </div>";

    html += "<h2>Basic settings</h2><p>Time format and other useful settings and tweaks.</p>";

    html += "<div class=\"row\"><label for=\"t_format\">Time format:</label>";
    html += "<select id=\"t_format\" name=\"t_format\">";
    html += "<option value=\"1\"";
    if (json["t_format"].as<int>() == 1) html += " selected";
    html += ">24-hour</option>";
    html += "<option value=\"0\"";
    if (!json["t_format"].isNull() && json["t_format"].as<int>() == 0) html += " selected";
    html += ">12-hour</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"zero\">Leading zero:</label>";
    html += "<select id=\"zero\" name=\"zero\">";
    html += "<option value=\"1\"";
    if (json["zero"].as<int>() == 1) html += " selected";
    html += ">Show</option>";
    html += "<option value=\"0\"";
    if (!json["zero"].isNull() && json["zero"].as<int>() == 0) html += " selected";
    html += ">Hide</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"rst_cycle\">Cycle through digits after every reset:</label>";
    html += "<select id=\"rst_cycle\" name=\"rst_cycle\">";
    html += "<option value=\"0\"";
    if (json["rst_cycle"].as<int>() == 0) html += " selected";
    html += ">No</option>";
    html += "<option value=\"1\"";
    if (json["rst_cycle"].as<int>() == 1) html += " selected";
    html += ">Yes (good for testing all digits)</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"rst_ip\">Show IP address after reset (last 3 digits):</label>";
    html += "<select id=\"rst_ip\" name=\"rst_ip\">";
    int rst_ip = json["rst_ip"].as<int>();
    html += "<option value=\"1\"";
    if (rst_ip == 1) html += " selected";
    html += ">Yes</option>";
    html += "<option value=\"0\"";
    if (!json["rst_ip"].isNull() && rst_ip == 0) html += " selected";
    html += ">No</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"ntp\">NTP server address or IP</label>";
    html += "<input type=\"text\" id=\"ntp\" name=\"ntp\" placeholder=\"";
    html += ntpServerName;
    html += "\" value=\"";
    html += json["ntp"].as<const char*>();
    html += "\">";
    html += "<p>Leave empty to use ";
    html += ntpServerName;
    html += "</p>";
    html += "</div>";


    html += "<h2>Visual settings</h2><p>Brightness, animations and other fun stuff.</p>";


    html += "<div class=\"row\"><label for=\"bri\">Brightness:</label>";
    html += "<select id=\"bri\" name=\"bri\">";
    html += "<option value=\"2\"";
    if (json["bri"].as<int>() == 2) html += " selected";
    html += ">High</option>";
    html += "<option value=\"1\"";
    if (json["bri"].as<int>() == 1) html += " selected";
    html += ">Medium</option>";
    html += "<option value=\"0\"";
    if (!json["bri"].isNull() && json["bri"].as<int>() == 0) html += " selected";
    html += ">Low</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"colon\">Colon:</label>";
    html += "<select id=\"colon\" name=\"colon\">";
    html += "<option value=\"2\"";
    if (json["colon"].as<int>() == 2) html += " selected";
    html += ">ON/OFF each second</option>";
    html += "<option value=\"0\"";
    if (!json["colon"].isNull() && json["colon"].as<int>() == 0) html += " selected";
    html += ">Off</option>";
    html += "<option value=\"1\"";
    if (json["colon"].as<int>() == 1) html += " selected";
    html += ">Always ON</option>";
#if !defined(CLOCK_VERSION_IV22)
    // gradient doesn't make sense on IV-22 colon
    html += "<option value=\"3\"";
    if (json["colon"].as<int>() == 3) html += " selected";
    html += ">Always ON (gradient)</option>";
#endif
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"fade\">Crossfade animation:</label>";
    html += "<select id=\"fade\" name=\"fade\">";
    html += "<option value=\"30\"";
    if (json["fade"].as<int>() == 30) html += " selected";
    html += ">Slow</option>";
    html += "<option value=\"20\"";
    if (json["fade"].as<int>() == 20) html += " selected";
    html += ">Medium</option>";
    html += "<option value=\"15\"";
    if (json["fade"].as<int>() == 15) html += " selected";
    html += ">Fast</option>";
    html += "<option value=\"0\"";
    if (!json["fade"].isNull() && json["fade"].as<int>() == 0) html += " selected";
    html += ">Disable</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"nmode\">Night mode:</label>";
    html += "<select id=\"nmode\" name=\"nmode\">";
    unsigned int nmode = json["nmode"].as<unsigned int>();
    html += "<option value=\"0\"";
    if (nmode == 0) html += " selected";
    html += ">None</option>";
    html += "<option value=\"1\"";
    if (nmode == 1) html += " selected";
    html += ">Set low brightness between 22:00-06:00</option>";
    //html += "<option value=\"2\"";
    //if (nmode == 2) html += " selected";
    //html += ">Clock OFF between 22:00-06:00</option>";
    html += "</select></div>";

    html += "<div class=\"row\"><label for=\"bal_enable\">";
    html += "<input type=\"checkbox\" id=\"bal_enable\" name=\"bal_enable\" value=\"1\" onClick=\"JavaScript:toggleVisibility(this,'bal_wrapper');\"";
    if (json["bal_enable"].as<int>() == 1) {
      html += " checked=\"checked\"";
    }
    html += ">&nbsp;Enable brightness balancing</label></div>";

    html += "<div id=\"bal_wrapper\">"; // Toggle wrapper start
    html += "<p>When buying used tubes, some are more severely burned out than others. Use options below to lower brightness of the good tubes to match the burned-out ones.</p>";


    html += "<h3>High brightness balance:</h3>";
    html += "<div class=\"flexrow\">"; // FLEX ROW START

    for (int i = 0; i < registersCount; i++) {
      html += "<div class=\"col\">"; // COLUMN START
      html += "<div class=\"row\"><label for=\"bal_h_" + String(i) + "\">Digit " + String(i + 1) + ":</label>";
      html += "<select id=\"bal_h_" + String(i) + "\" name=\"bal_h_" + String(i) + "\">";

      for (int ii = pwmResolution; ii > 0; ii -= dimmingSteps) {
        html += "<option value=\"" + String(ii) + "\"";
        if ((!json["bal"]["high"][i].isNull() && json["bal"]["high"][i].as<int>() == ii) || (json["bal"]["low"][i].isNull() && ii == pwmResolution)) html += " selected";
        html += ">" + String(ii) + "</option>";
      }

      html += "</select></div>";
      html += "</div>"; // COLUMN END
    }
    html += "</div>"; // FLEX ROW END

    html += "<h3>Low brightness balance:</h3>";
    html += "<div class=\"flexrow\">"; // FLEX ROW START
    for (int i = 0; i < registersCount; i++) {
      html += "<div class=\"col\">"; // COLUMN START
      html += "<div class=\"row\"><label for=\"bal_l_" + String(i) + "\">Digit " + String(i + 1) + ":</label>";
      html += "<select id=\"bal_l_" + String(i) + "\" name=\"bal_l_" + String(i) + "\">";

      for (int ii = pwmResolution; ii > 0; ii -= dimmingSteps) {
        html += "<option value=\"" + String(ii) + "\"";
        if ((!json["bal"]["low"][i].isNull() && json["bal"]["low"][i].as<int>() == ii) || (json["bal"]["low"][i].isNull() && ii == dimmingSteps)) html += " selected";
        html += ">" + String(ii) + "</option>";
      }

      html += "</select></div>";
      html += "</div>"; // COLUMN END
    }
    html += "</div>"; // FLEX ROW END

    html += "</div>"; // Toggle wrapper end



    html += "<h2>Timezone settings</h2>";
    html += "<p>Set your timezone and optionally also daylight saving.</p>";


    html += "<div class=\"row\"><label for=\"std_offset\">Standard time UTC offset (in minutes, -660 = -11h, 660 = +11h):</label>";
    html += "<input type=\"number\" id=\"std_offset\" name=\"std_offset\" min=\"-660\" max=\"660\" value=\"";
    html += json["std_offset"].as<const char*>();
    html += "\"></div>";

    html += "<div class=\"row\"><label for=\"dst_enable\">";
    html += "<input type=\"checkbox\" id=\"dst_enable\" name=\"dst_enable\" value=\"1\" onClick=\"JavaScript:toggleVisibility(this,'dst_wrapper');\"";
    if (json["dst_enable"].as<int>() == 1) {
      html += " checked=\"checked\"";
    }
    html += ">&nbsp;Enable Daylight Saving (summer time)</label></div>";
    html += "<p>When enabled, additional fields will pop up for you to set daylight saving time. <a href=\"https://www.timeanddate.com/time/map/\" target=\"_blank\">You can search for your timezone here</a></p>";



    html += "<div id=\"dst_wrapper\">"; // WRAPPER START

    html += "<h3>Standard (winter) time starts at:</h3>";

    html += "<div class=\"flexrow\">"; // FLEX ROW START

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_week\">Week:</label>";
    html += "<select id=\"std_week\" name=\"std_week\">";
    unsigned int std_week = json["std_week"].as<unsigned int>();
    html += "<option value=\"0\"";
    if (std_week == 0) html += " selected";
    html += ">Last</option>";
    html += "<option value=\"1\"";
    if (std_week == 1) html += " selected";
    html += ">First</option>";
    html += "<option value=\"2\"";
    if (std_week == 2) html += " selected";
    html += ">Second</option>";
    html += "<option value=\"3\"";
    if (std_week == 3) html += " selected";
    html += ">Third</option>";
    html += "<option value=\"4\"";
    if (std_week == 4) html += " selected";
    html += ">Fourth</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_day\">Day:</label>";
    html += "<select id=\"std_day\" name=\"std_day\">";
    unsigned int std_day = json["std_day"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (std_day == 1) html += " selected";
    html += ">Sun</option>";
    html += "<option value=\"2\"";
    if (std_day == 2) html += " selected";
    html += ">Mon</option>";
    html += "<option value=\"3\"";
    if (std_day == 3) html += " selected";
    html += ">Tue</option>";
    html += "<option value=\"4\"";
    if (std_day == 4) html += " selected";
    html += ">Wed</option>";
    html += "<option value=\"5\"";
    if (std_day == 5) html += " selected";
    html += ">Thu</option>";
    html += "<option value=\"6\"";
    if (std_day == 6) html += " selected";
    html += ">Fri</option>";
    html += "<option value=\"7\"";
    if (std_day == 7) html += " selected";
    html += ">Sat</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_month\">Month:</label>";
    html += "<select id=\"std_month\" name=\"std_month\">";
    unsigned int std_month = json["std_month"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (std_month == 1) html += " selected";
    html += ">Jan</option>";
    html += "<option value=\"2\"";
    if (std_month == 2) html += " selected";
    html += ">Feb</option>";
    html += "<option value=\"3\"";
    if (std_month == 3) html += " selected";
    html += ">Mar</option>";
    html += "<option value=\"4\"";
    if (std_month == 4) html += " selected";
    html += ">Apr</option>";
    html += "<option value=\"5\"";
    if (std_month == 5) html += " selected";
    html += ">May</option>";
    html += "<option value=\"6\"";
    if (std_month == 6) html += " selected";
    html += ">Jun</option>";
    html += "<option value=\"7\"";
    if (std_month == 7) html += " selected";
    html += ">Jul</option>";
    html += "<option value=\"8\"";
    if (std_month == 8) html += " selected";
    html += ">Aug</option>";
    html += "<option value=\"9\"";
    if (std_month == 9) html += " selected";
    html += ">Sep</option>";
    html += "<option value=\"10\"";
    if (std_month == 10) html += " selected";
    html += ">Oct</option>";
    html += "<option value=\"11\"";
    if (std_month == 11) html += " selected";
    html += ">Nov</option>";
    html += "<option value=\"12\"";
    if (std_month == 12) html += " selected";
    html += ">Dec</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"std_hour\">Hour:</label>";
    html += "<input type=\"number\" id=\"std_hour\" name=\"std_hour\" min=\"0\" max=\"23\" value=\"";
    html += json["std_hour"].as<const char*>();
    html += "\"></div>";
    html += "</div>"; // COLUMN END

    html += "</div>"; // FLEX ROW END







    html += "<h3>Daylight saving (summer) time starts at:</h3>";

    html += "<div class=\"flexrow\">"; // FLEX ROW START

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_week\">Week:</label>";
    html += "<select id=\"dst_week\" name=\"dst_week\">";
    unsigned int dst_week = json["dst_week"].as<unsigned int>();
    html += "<option value=\"0\"";
    if (dst_week == 0) html += " selected";
    html += ">Last</option>";
    html += "<option value=\"1\"";
    if (dst_week == 1) html += " selected";
    html += ">First</option>";
    html += "<option value=\"2\"";
    if (dst_week == 2) html += " selected";
    html += ">Second</option>";
    html += "<option value=\"3\"";
    if (dst_week == 3) html += " selected";
    html += ">Third</option>";
    html += "<option value=\"4\"";
    if (dst_week == 4) html += " selected";
    html += ">Fourth</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_day\">Day:</label>";
    html += "<select id=\"dst_day\" name=\"dst_day\">";
    unsigned int dst_day = json["dst_day"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (dst_day == 1) html += " selected";
    html += ">Sun</option>";
    html += "<option value=\"2\"";
    if (dst_day == 2) html += " selected";
    html += ">Mon</option>";
    html += "<option value=\"3\"";
    if (dst_day == 3) html += " selected";
    html += ">Tue</option>";
    html += "<option value=\"4\"";
    if (dst_day == 4) html += " selected";
    html += ">Wed</option>";
    html += "<option value=\"5\"";
    if (dst_day == 5) html += " selected";
    html += ">Thu</option>";
    html += "<option value=\"6\"";
    if (dst_day == 6) html += " selected";
    html += ">Fri</option>";
    html += "<option value=\"7\"";
    if (dst_day == 7) html += " selected";
    html += ">Sat</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_month\">Month:</label>";
    html += "<select id=\"dst_month\" name=\"dst_month\">";
    unsigned int dst_month = json["dst_month"].as<unsigned int>();
    html += "<option value=\"1\"";
    if (dst_month == 1) html += " selected";
    html += ">Jan</option>";
    html += "<option value=\"2\"";
    if (dst_month == 2) html += " selected";
    html += ">Feb</option>";
    html += "<option value=\"3\"";
    if (dst_month == 3) html += " selected";
    html += ">Mar</option>";
    html += "<option value=\"4\"";
    if (dst_month == 4) html += " selected";
    html += ">Apr</option>";
    html += "<option value=\"5\"";
    if (dst_month == 5) html += " selected";
    html += ">May</option>";
    html += "<option value=\"6\"";
    if (dst_month == 6) html += " selected";
    html += ">Jun</option>";
    html += "<option value=\"7\"";
    if (dst_month == 7) html += " selected";
    html += ">Jul</option>";
    html += "<option value=\"8\"";
    if (dst_month == 8) html += " selected";
    html += ">Aug</option>";
    html += "<option value=\"9\"";
    if (dst_month == 9) html += " selected";
    html += ">Sep</option>";
    html += "<option value=\"10\"";
    if (dst_month == 10) html += " selected";
    html += ">Oct</option>";
    html += "<option value=\"11\"";
    if (dst_month == 11) html += " selected";
    html += ">Nov</option>";
    html += "<option value=\"12\"";
    if (dst_month == 12) html += " selected";
    html += ">Dec</option>";
    html += "</select></div>";
    html += "</div>"; // COLUMN END

    html += "<div class=\"col\">"; // COLUMN START
    html += "<div class=\"row\"><label for=\"dst_hour\">Hour:</label>";
    html += "<input type=\"number\" id=\"dst_hour\" name=\"dst_hour\" min=\"0\" max=\"23\" value=\"";
    html += json["dst_hour"].as<const char*>();
    html += "\"></div>";
    html += "</div>"; // COLUMN END

    html += "</div>"; // FLEX ROW END

    html += "<div class=\"row\"><label for=\"dst_offset\">Daylight saving (summer) time UTC offset (in minutes):</label>";
    html += "<input type=\"number\" id=\"dst_offset\" name=\"dst_offset\" min=\"-660\" max=\"660\" value=\"";
    html += json["dst_offset"].as<const char*>();
    html += "\"></div>";

    html += "</div>"; // WRAPPER END

    // This is just to check if it's a remote GET request or full update.
    html += "<input type=\"hidden\" name=\"is_form\" value=\"1\">";

    html += "<div class=\"row\"><button type=\"submit\">Save and reboot</button></div></form>";
    html += FPSTR(html_footer);

    html.replace("{{fw_name}}", FW_NAME);
    html.replace("{{fw_version}}", FW_VERSION);

    server.send(200, "text/html", html);

  } else {
    for (uint8_t i = 0; i < server.args(); i++) {
      html += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(200, "text/plain", html);
  }

  Serial.print("[SERV] Serving HTML took: ");
  Serial.print(millis() - serverProcessingTime);
  Serial.println(" ms");

  if (server.args() && server.hasArg("is_form")) {
    delay(1000);
    ESP.restart();
    delay(100);
  }

}
