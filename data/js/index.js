function showTemperature() {
    $.post("/temperature");
}

function showHumidity() {
    $.post("/humidity");
}

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("vlhkost").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/vlhkost", true);
    xhttp.send();
  }, 1000);

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
        document.getElementById("teplota").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "/teplota", true);
    xhttp.send();
  }, 1000);