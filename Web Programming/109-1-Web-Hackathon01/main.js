var cells = document.getElementsByClassName("date");
var input = document.getElementById("cal-input");
var color = document.getElementById("cal-color");
var button = document.getElementById("cal-button");
var now_date_e = 0;
var now_date = 0

color.value = '#b0b0b0'

function get(id){
    if(now_date_e != 0){
        now_date_e.classList.remove("date_clicked");
    }
    var date = document.getElementById(id);
    date.classList.add("date_clicked");
    now_date_e = date;
    now_date = id;
}

for(i = 0; i < 28; i++)
    cells[i].setAttribute("onClick", "get(this.id)");

function content(word){
    var p = document.createElement("P");
    now_date_e.appendChild(p);
    p.style.color=color.value;
    p.innerHTML = word;
}

function add(){
    content(input.value);
    input.value = "";
}

input.addEventListener('keyup', event => {
    if (event.keyCode === 13 && event.target.value !== '') {
        content(input.value);
        input.value = "";
    }
    });


//Sets the page's theme. No need to modify
var themeButton = document.getElementsByClassName("ChooseTheme")
for(var i=0; i<themeButton.length; ++i) {
    themeButton[i].addEventListener('click', e => {
        document.body.setAttribute('class', e.target.id)
    }, false)
}
