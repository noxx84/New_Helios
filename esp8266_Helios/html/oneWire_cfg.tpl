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

var counter=0;

var counter_max=5;

var sensors = [
    "Sensor_Solar",	
    "Sensor_Boiler_UP",
    "Sensor_Boiler_Middle",
    "Sensor_Boiler_Down",
    "Sensor_Stove"
];
function createDivider() {
  var divider=document.createElement("li");
  divider.className="ui-li-divider ui-bar-inherit ui-first-child";
  divider.textContent="Sellect next in case sensor is correctly recognised:";
  return divider;
}

function getNextSensor() {
  if(counter<counternIter++) {
  counter++;
  return counter
  }
  else return -1;
}


function createInputForAp(ap) {
  var element=document.createElement("li");
  var sensor=document.createElement("a");
  SensorName.className="ui-btn ui-btn-icon-right ui-icon-carat-r";
  var SensorName = "sensor"+sensors[app.current]+ap.status;
  SensorName.id=SensorName;
  SensorName.textContent=ap.status;
  element.appendChild(SensorName);
  return element;
}

function scanSensors() {
  var sensor_pos=getNextSensor();
  xhr.open("GET", "cgiOneWire_cfg.cgi?sensor=sensor_pos");
  xhr.onreadystatechange=function() {
    if (xhr.readyState==4 && xhr.status>=200 && xhr.status<300) {
      var data=JSON.parse(xhr.responseText);
      if (data.result.inProgress=="0") {
        $("#sensor").innerHTML="";
        $("#sensor").appendChild(createDivider());
          networks.push(data.result.APs[0].status);
          $("#sensor").appendChild(createInputForAp(data.result.APs[0]));
        }
      } 
    }
  }
  xhr.send();
}
</script>
</head>
<body class="ui-mobile-viewport ui-overlay-a">
  <div data-role="page" id="pageone" data-url="pageone" tabindex="0" class="ui-page ui-page-theme-a ui-page-active" style="min-height: 459px;">
	<div data-role="header" role="banner" class="ui-header ui-bar-inherit">
		<h1 class="ui-title" role="heading" aria-level="1">Helios_New</h1>
	</div>
	<div data-role="main" class="ui-content">
		<ul id="aps" data-role="listview" data-inset="true" class="ui-listview ui-listview-inset ui-corner-all ui-shadow">
      <li><a href="#" class="ui-btn ui-btn-inner ui-corner-all ui-shadow" style="text-align: center;">Scanning... Please wait...</a></li>
		</ul>
	</div>

	<div class="inputSection">
		

	<button onclick="scanSensors()">Configure Sensors</button> 
	
    <form name="OneConfigForm" action="cgiOneWire.cgi" method="post">

    <div data-role="collapsible" onclick="collapse('advanced', 'advancedIcon');" data-collapsed-icon="carat-d" data-expanded-icon="carat-u" class="ui-collapsible ui-collapsible-inset ui-corner-all ui-collapsible-themed-content ui-collapsible-collapsed">
      <h4 class="ui-collapsible-heading ui-collapsible-heading-collapsed">
        <a href="#" id="advancedIcon" class="ui-collapsible-heading-toggle ui-btn ui-btn-icon-left ui-btn-inherit ui-icon-carat-d">Advanced</a></h4>
        <div id="advanced" class="ui-collapsible-content ui-body-inherit ui-collapsible-content-collapsed">

        <label for="basic">SET Point Boiler : </label>
    	<div class="ui-input-text ui-body-inherit ui-corner-all ui-shadow-inset">
	<input type="text" name="set_point_A_boiler" value="" placeholder="set point A" maxlength="100">
        <input type="text" name="set_point_B_boiler" value="" placeholder="set point B" maxlength="100">
    	</div>

    	<label for="basic">SET Point Stove  </label>
	<div class="ui-input-text ui-body-inherit ui-corner-all ui-shadow-inset">
        <input type="text" name="set_point_A_stove" value="" placeholder="set point A" maxlength="100">
        <input type="text" name="set_point_B_stove" value="" placeholder="set point B" maxlength="100">
    	</div>
    	
	<label for="basic">SET Outputs for control functions  </label>
	<div class="ui-input-text ui-body-inherit ui-corner-all ui-shadow-inset">
        Setup (between 0 and 7):<input type="number" name="Pomp" min="0" max="7"  value="" placeholder="0" maxlength="100">
        Setup (between 0 and 7):<input type="number" name="Valve" min="0" max="7" value="" placeholder="1" maxlength="100">
    	</div>
        </div>
    </div>


    <div class="ui-input-btn ui-btn ui-corner-all ui-shadow">Configure
      <input type="submit" name="Configure" value="Configure" />
    </div>

    </form>
	</div>


  </div>
</body>
</html>
