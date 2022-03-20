var previousbutton = document.getElementById("previous");
var nextbutton = document.getElementById("next");
var image = document.getElementById("display");
var source = document.getElementById("source");
var imagebox = [
    "https://www.teepr.com/wp-content/uploads/2019/06/15533156982868.jpg",
    "https://assets.juksy.com/files/articles/77737/800x_100_w-5ad6c9e143014.jpg",
    "https://cdn.bella.tw/files/Bella-Bodhi-Nike-07(1).jpg",
    "https://assets.juksy.com/files/articles/77737/800x_100_w-5ad6c9e1431b7.jpg",
    "https://image.japaholic.com/article/images/2015/08/Pouche69fb4e78aace381bee3828bmain-e1438658935362.jpg",
    "https://girlsmood.com/web/wp-content/uploads/2020/08/mirimogu_67566118_880668288983305_7554891598115026945_n.jpg",
    "https://angelala.tw/wp-content/uploads/img/20170419083656_22.jpg"
];
var now = 0;

function previousImage(){
    if(now > 0){
        image.src = "images/loading.gif";
        if(now == imagebox.length - 1){
            nextbutton.classList.remove("disabled")}
        now -= 1;
        image.src = imagebox[now];
        source.innerHTML = ("Source:" + imagebox[now]);  
    }
    if(now == 0){
        previousbutton.classList.add("disabled");}
}



function nextImage(){
    if(now < imagebox.length-1){
        image.src = "images/loading.gif";
        if(now == 0){
            previousbutton.classList.remove("disabled")}
        now += 1;
        image.src = imagebox[now];
        source.innerHTML = ("Source:" + imagebox[now]);
    }
    if(now == imagebox.length-1){
        nextbutton.classList.add("disabled");}
}





