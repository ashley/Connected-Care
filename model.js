var chartHeight = parseInt(document.getElementById('graph').style.height);
var chartWidth = parseInt(document.getElementById('graph').style.width);

var ceiling = 200;
var y = d3.scale.linear().domain([0, ceiling]).range([0, chartHeight]);

function displayStackedChart(chartId) {
	var vis = d3.select("#" + chartId).append("svg:svg").attr("width", "100%").attr("height", "100%")
	.append("g").attr("class","barChart").attr("transform", "translate(0, " + chartHeight + ")");
}

var propertyNames = ["a", "b", "c"];

function addData(chartId, data) {
	var existingBarNode = document.querySelectorAll("#" + chartId + "_" + data.id);
	if(existingBarNode.length > 0) {
		var existingBar = d3.select(existingBarNode.item());
		existingBar.transition().duration(100)
			.attr("style", "opacity:1.0");
		for(index in propertyNames) {
			existingBar.select("rect." + propertyNames[index])
				.transition().ease("linear").duration(300)
				.attr("y", barY(data, propertyNames[index]))
				.attr("height", barHeight(data, propertyNames[index]));
		}
	} else {
		var barDimensions = updateBarWidthsAndPlacement(chartId);
		var barGroup = d3.select("#" + chartId).selectAll("g.barChart")
			.append("g")
				.attr("class", "bar")
				.attr("id", chartId + "_" + data.id)
				.attr("style", "opacity:1.0");

		for(index in propertyNames) {
			barGroup.append("rect")
				.attr("class", propertyNames[index])
			    .attr("width", (barDimensions.barWidth-1))
			    .attr("x", function () { return (barDimensions.numBars-1) * barDimensions.barWidth;})
			    .attr("y", barY(data, propertyNames[index]))
			    .attr("height", barHeight(data, propertyNames[index]));
		}

		barGroup.styleInterval = setInterval(function() {
				var theBar = document.getElementById(chartId + "_" + data.id);
				if(theBar == undefined) {
					clearInterval(barGroup.styleInterval);
				} else {
					if(theBar.style.opacity > 0.2) {
						theBar.style.opacity = theBar.style.opacity - 0.05;
					}
				}
			}, 1000);

	}
}

function removeData(chartId, barId) {
	var existingBarNode = document.querySelectorAll("#" + chartId + "_" + barId);
	if(existingBarNode.length > 0) {
		var barGroup = d3.select(existingBarNode.item());
		barGroup
			.transition().duration(200)
			.remove();
	}
}

function updateBarWidthsAndPlacement(chartId) {
	var numBars = document.querySelectorAll("#" + chartId + " g.bar").length + 1;
	var barWidth = chartWidth/numBars;
	if(barWidth > 50) {
		barWidth=50;
	}

	var barNodes = document.querySelectorAll(("#" + chartId + " g.barChart g.bar"));
	for(var i=0; i < barNodes.length; i++) {
		d3.select(barNodes.item(i)).selectAll("rect")
			//.transition().duration(10) // animation makes the display choppy, so leaving it out
			.attr("x", i * barWidth)
			.attr("width", (barWidth-1));
	}

	return {"barWidth":barWidth, "numBars":numBars};
}

function barY(data, propertyOfDataToDisplay) {
	var baseline = 0;
	for(var j=0; j < index; j++) {
		baseline = baseline + data[propertyNames[j]];
	}
	return -y(baseline + data[propertyOfDataToDisplay]);
}

function barHeight(data, propertyOfDataToDisplay) {
	return data[propertyOfDataToDisplay];
}

function randomInt(magnitude) {
	return Math.floor(Math.random()*magnitude);
}


displayStackedChart("graph");
var click,temp;

function start() {
		document.getElementById("uptime").innerHTML = "Waiting for data...";
		var deviceID = "1c003e000447353138383138";
		var accessToken = "f002ebd44f47935f18cedf319fd1f657c3dd2c34";
		var eventSource = new EventSource("https://api.spark.io/v1/devices/" + deviceID + "/events/?access_token=" + accessToken);

		eventSource.addEventListener('open', function(e) {
				console.log("Opened!"); },false);

		eventSource.addEventListener('error', function(e) {
				console.log("Errored!"); },false);

		eventSource.addEventListener('ButtonClick', function(e) {
				var parsedData = JSON.parse(e.data);
				var tempSpan = document.getElementById("uptime");
				var tsSpan   = document.getElementById("tstamp");
				tempSpan.innerHTML = parsedData.data;
				var arr = parsedData.data.split(', ');
				click = parseFloat(arr[2]);
				temp = parseFloat(arr[1]);
				tempSpan.style.fontSize = "28px";
				tsSpan.innerHTML = "At timestamp " + parsedData.published_at;
				tsSpan.style.fontSize = "9px";
				if(click == 0){
					document.getElementById("status").innerHTML = "Good";
					document.getElementById("status").style.color = "Green";
					console.log("Good");
				}
				else if(click ==1){
					document.getElementById("status").innerHTML = "Not Good. In danger";
					document.getElementById("status").style.color = "Red";
					console.log("Bad");
				}
				else{
					console.log("NaN");
				}
		}, false);



		setInterval(function() {
				addData("graph", {"id":"v"+randomInt(150), "a":temp, "b":0, "c":0});
			}, 5000);
}

var i = 1;
function demoStart() {
	setInterval(function() {
		console.log(i);
		i++;
		addData("graph", {"id":"v"+randomInt(150), "a":rawdata.a[i], "b":rawdata.a[i]+10, "c":rawdata.a[i]+20});
		console.log(rawdata.a[i]+rawdata.b[i]+rawdata.c[i]);
		if(rawdata.a[i]+rawdata.b[i]+rawdata.c[i] <=90){
			document.getElementById("status").innerHTML = "Good";
			document.getElementById("status").style.color = "Green";
			console.log("Good");
			document.getElementById("off").checked = true;
			document.getElementById("on").checked = false;
			document.getElementById("alert").submit();
			document.forms["alert1"].submit();



		}
		else{
			document.getElementById("status").innerHTML = "Not Good. In danger";
			document.getElementById("status").style.color = "Red";
			console.log("Bad");
			document.getElementById("on").checked = true;
			document.getElementById("off").checked = false;
			document.getElementById("alert").submit();
			document.forms["alert1"].submit();
			document.getElementById("deleteButton").click();
		}	
				
	}, 1000);
}

/* kick off a continual interval timer to simulate the occasional removal of data */
setInterval(function() {
	// we want removals to be somewhat bursty, so we'll randomize how many we remove
	var numToRemove = randomInt(20);
	for(var r=0; r<numToRemove; r++) {
		removeData("graph", "v"+randomInt(150));
	}
}, 2000);

function tabChange(evt, pageName) {
    // Declare all variables
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }

    document.getElementById(pageName).style.display = "block";
    evt.currentTarget.className += " active";
}

function optionTest(){
    actors_list  = document.getElementById("actors_list");
    actor_to_actor.forEach(function(key,value){
       var option = document.createElement('option');
       option.value = value;
       actors_list.appendChild(option);
    });
}

