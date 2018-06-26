function handleFiles(files) {
	// Check for the various File API support.
	if (window.FileReader) {
		// FileReader are supported.
		getAsText(files[0]);
	} else {
		alert('FileReader are not supported in this browser.');
	}

        document.getElementById("fileStatus").innerHTML = 'File ' + document.getElementById("csvFileInput").value  + ' loaded with sucess! You can still append more TS or samples.';
        document.getElementById("csvFileInput").value = "";
}

function getAsText(fileToRead) {
	var reader = new FileReader();
	// Handle errors load
	reader.onload = loadHandler;
	reader.onerror = errorHandler;
	// Read file into memory as UTF-8      
	reader.readAsText(fileToRead);	
}

function loadHandler(event) {
	var csv = event.target.result;
	processData(csv);             
}


function processData(csv) {
    //var allTextLines = csv.split(/\r\n|\n/);
    var allTextLines = csv.match(/[^\r\n]+/g); 
    var lines = [];
    while (allTextLines.length) {
        lines.push(allTextLines.shift().split(','));
    }

	console.log(lines);
	drawOutput(lines,"output");
	loadTS = Module.cwrap('loadTS','void', ['string','number'])

        console.log(lines.length)
        console.log(allTextLines.length)

	for (var i = 1; i < lines.length; i++) {
		loadTS(lines[i][0],lines[i][0].length,0);
		loadTS(lines[i][1],lines[i][1].length,1);
	}

	// Updating div with TS status
	document.getElementById("timeTSStatus").innerHTML = '';
	getNTimeSeries = Module.cwrap('getNTimeSeries','number', ['void'])

	var tsSize = getNTimeSeries();

	var mydiv = document.getElementById("timeTSStatus");
        var newcontent = document.createElement('div');
        newcontent.innerHTML = 'Cache contains ' + tsSize + ' time series with sizes:';

        while (newcontent.firstChild) {
              mydiv.appendChild(newcontent.firstChild);
        }
	getTimeSeriesSize = Module.cwrap('getTimeSeriesSize','number', ['number'])
        for (var i = 0; i < tsSize; i++) {  
       		newcontent = document.createElement('div');
        	newcontent.innerHTML = ' TS['+i +']:' + getTimeSeriesSize(i);
   		while (newcontent.firstChild) {
              		mydiv.appendChild(newcontent.firstChild);
        	}
        }
	// Updating div with TS status
}

function errorHandler(evt) {
	if(evt.target.error.name == "NotReadableError") {
		alert("Canno't read file !");
	}
}

function drawOutput(lines,id){
	//Clear previous data
	document.getElementById(id).innerHTML = "";
	var table = document.createElement("table");
	for (var i = 0; i < lines.length; i++) {
		var row = table.insertRow(-1);
		for (var j = 0; j < lines[i].length; j++) {
			var firstNameCell = row.insertCell(-1);
			firstNameCell.appendChild(document.createTextNode(lines[i][j]));
		}
	}
	document.getElementById(id).appendChild(table);
}

function processForecasts(csvForecasts) {
    var allTextLines = csvForecasts.split(/\r\n|\n/);
    var lines = [];
    while (allTextLines.length) {
        lines.push(allTextLines.shift().split(','));
    }
    console.log(lines);
    drawOutput(lines,"outputForecasts");
}
