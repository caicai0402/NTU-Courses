var canvas;
var bg;
var rain;

var bird;
var bird_tag;
var bird_music;
var bird_slider;

var dog;
var dog_tag;
var dog_music;
var dog_slider;

var drum;
var drum_tag;
var drum_music;
var drum_slider;

function setup() {
  canvas = createCanvas(1200, 800);
  canvas.position((windowWidth - width) / 2, (windowHeight - height) / 2);
  
  bg = loadImage('assets/nature.jpg');
  rain = loadImage('assets/rain.gif');

  bird = createImg('assets/bird.jpg');
  bird.size(200, 200)
  bird.position(4*windowWidth/7, 3*windowHeight/7);
  bird.mousePressed(bire_fn);
  bird_tag = 0;
  bird_slider = createSlider(0, 255, 0);
  bird_slider.position(4*windowWidth/7, 3*windowHeight/5); //450, 750
  bird_slider.style('width', '80px');
  bird_slider.input(mysend);

  dog = createImg('assets/dog.jpg');
  dog.size(180, 250)
  dog.position(4*windowWidth/13, 3*windowHeight/7);
  dog.mousePressed(dog_fn);
  dog_tag = 0;
  dog_slider = createSlider(0, 255, 0);
  dog_slider.position(4*windowWidth/13, 3*windowHeight/5); // 150, 750
  dog_slider.style('width', '80px');
  dog_slider.input(mysend);

  drum = createImg('assets/drum.jpg');
  drum.size(250, 180)
  drum.position(8*windowWidth/19, 3*windowHeight/4);
  drum.mousePressed(drum_fn);
  drum_tag = 0;
  drum_slider = createSlider(0, 255, 0);
  drum_slider.position(8*windowWidth/19, 4*windowHeight/5); // 300, 750
  drum_slider.style('width', '80px');
  drum_slider.input(mysend);
}

function draw() {
  background(bg);
  image(rain, 0, 0, 600, 800);
  image(rain, 600, 0, 600, 800);  
}


function bire_fn() {
  if (bird_tag == 0) {
    bird_music = createImg('assets/bird_music.jpg');
    bird_music.position(4*windowWidth/8, 1*windowHeight/8);
    bird_music.size(240, 320);
    bird_tag = 1;
  }
  else {
    bird_music.remove()
    bird_tag = 0;
  }
  mysend();
}

function dog_fn() {
  if (dog_tag == 0) {
    dog_music = createImg('assets/dog_music.jpg');
    dog_music.position(3*windowWidth/8, windowHeight/9);
    dog_music.size(150, 300);
    dog_tag = 1;
  }
  else {
    dog_music.remove()
    dog_tag = 0;
  }
  mysend();
}

function drum_fn() {
  if (drum_tag == 0) {
    drum_music = createImg('assets/drum_music.jpg');
    drum_music.position(23*windowWidth/50, 4*windowHeight/10);
    drum_music.size(130, 330);
    drum_tag = 1;
  }
  else {
    drum_music.remove()
    drum_tag = 0;
  }
  mysend();
}

function mysend(){
  let url = "http://localhost:9000";
  let data = "".concat(bird_tag.toString(), ' ', (bird_slider.value()).toString(), ' ', dog_tag.toString(), ' ', (dog_slider.value()).toString(), ' ', drum_tag.toString(), ' ', (drum_slider.value()).toString());
  // httpPost(url, "text", "gg", function(result) {bird.position(mouseX, mouseY);}, function(result){dog.position(mouseX, mouseY);});
  httpPost("http://localhost:9000", "text", data, function(result) {}, function(result){});
}