<!DOCTYPE html>
<html class="ui-mobile">
<head>
<title>OneWire Config</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="jquery.mobile-1.4.5.min.css">
<link rel="stylesheet" href="style.css">
<script type="text/javascript" src="140medley.min.js"></script>
<script type="text/javascript" src="common.js"></script>
<script type="text/javascript">

var xhr=j();

var currAp="%Sensor%";

var sensors=[];

function createDivider() {
  var divider=document.createElement("li");
  divider.className="ui-li-divider ui-bar-inherit ui-first-child";
  divider.textContent="Please select your WiFi network:";
  return divider;
}

function createInputForAp(ap) {
  var element=document.createElement("li");
  var sensor=document.createElement("a");
  sensor.className="ui-btn ui-btn-icon-right ui-icon-carat-r";
  var sensor_info = "sensor"+ap.name;
  sensor.id=sensor_info;
  sensor.textContent=ap.temp;
  element.appendChild(sensor);
  return element;
}

function swapFields(fieldOne, fieldTwo) {
    document.getElementById(fieldOne).style.display = 'block';
    document.getElementById(fieldTwo).style.display = 'none';
}
function OnChangeCheckbox (myonoffswitch) {
            if (myonoffswitch.checked) {
                xhr.open("GET", "cgiOneWireManual.cgi.cgi?value=ON");
            }
            else {
               xhr.open("GET", "cgiOneWire_read_temp.cgi?value=OFF");
            }
}

function scanTemp() {
  xhr.open("GET", "cgiOneWire_read_temp.cgi");
  xhr.onreadystatechange=function() {
    if (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {
      var data=JSON.parse(xhr.responseText);
      if (data.result.inProgress=="0" && data.result.APs.length>1) {
        $("#sensors").innerHTML="";
        $("#sensors").appendChild(createDivider());
        for (var i=0; i<data.result.APs.length; i++) {
          if (data.result.APs[i].name=="" && data.result.APs[i].temp==0) {
            continue;
          }
          networks.push(data.result.APs[i].temp);
          $("#aps").appendChild(createInputForAp(data.result.APs[i]));
        }
        window.setTimeout(scanAPs, 20000);
      } else {
        window.setTimeout(scanAPs, 1000);
      }
    }
  }
  xhr.send();
}

window.onload=function(e) {
  scanTemp();
};
</script>
</head>
<body class="ui-mobile-viewport ui-overlay-a">
  <div data-role="page" id="pageone" data-url="pageone" tabindex="0" class="ui-page ui-page-theme-a ui-page-active" style="min-height: 459px;">
	<div data-role="header" role="banner" class="ui-header ui-bar-inherit">
		<h1 class="ui-title" role="heading" aria-level="1">Sensor Readings</h1>
	</div>
	<div data-role="main" class="ui-content">
		<ul id="aps" data-role="listview" data-inset="true" class="ui-listview ui-listview-inset ui-corner-all ui-shadow">
      <li><a href="#" class="ui-btn ui-btn-inner ui-corner-all ui-shadow" style="text-align: center;">Scanning... Please wait...</a></li>
		</ul>
	</div>

	<div class="inputSection">
    <form name="TempFOrm" action="cgiOneWireManual.cgi" method="post">
    <div class="onoffswitch">
        <input type="checkbox" name="onoffswitch" onchange = "change(this)" class="onoffswitch-checkbox" id="myonoffswitch" checked>
        <label class="onoffswitch-label" for="myonoffswitch">
            <span class="onoffswitch-inner"></span>
            <span class="onoffswitch-switch"></span>
        </label>
    </div>


    <div class="ui-input-btn ui-btn ui-corner-all ui-shadow">Connect
      <input type="submit" name="connect" value="Connect" />
    </div>

    </form>
	</div>


  </div>
</body>
</html>
