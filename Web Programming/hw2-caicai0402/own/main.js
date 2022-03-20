const input = document.getElementById("todo-input");
const root = document.getElementById("root");
const list = document.getElementById("todo-list");

var t = 0;

var All_box = [];
var Active_box = [];
var Completed_box = [];

function addfooter(){
    const footer = document.createElement("FOOTER");
    const left = document.createElement("DIV");
    const view_buttons = document.createElement("UL");
    const all = document.createElement("BUTTON");
    const active = document.createElement("BUTTON");
    const completed = document.createElement("BUTTON");
    const clear_completed = document.createElement("DIV");
    const clean = document.createElement("BUTTON");
    footer.setAttribute("class", "todo-app__footer");
    footer.setAttribute("id", "todo-footer");
    left.setAttribute("class", "todo-app__total");
    left.setAttribute("id", "left");
    view_buttons.setAttribute("class", "todo-app__view-buttons");
    all.setAttribute("onClick", "all_f()");
    active.setAttribute("onClick", "active_f()");
    completed.setAttribute("onClick", "completed_f()");
    clear_completed.setAttribute("class", "todo-app__clean");
    clean.setAttribute("onClick", "clean_f()");
    root.appendChild(footer);
    footer.appendChild(left);
    footer.appendChild(view_buttons);
    view_buttons.appendChild(all);
    view_buttons.appendChild(active);
    view_buttons.appendChild(completed);
    footer.appendChild(clear_completed);
    clear_completed.appendChild(clean);
    all.innerHTML = "all";
    active.innerHTML = "active";
    completed.innerHTML = "completed";
    clean.innerHTML = "Clear completed";
}

function remove_f(){
    while (list.firstChild)
        list.removeChild(list.firstChild);
}

function all_f(){
    remove_f();
    for(i = 0; i < All_box.length; i++)
        if(All_box[i] != 0){
            if(Active_box.indexOf(i+1) > -1)
                someCodeToCreateNewItem(All_box[i], i+1);

            else if(Completed_box.indexOf(i+1) > -1){
                someCodeToCreateNewItem(All_box[i], i+1);   
                var temp = document.getElementById(i+1);
                temp.checked = true;
                temp = document.getElementById(i+1+"h");
                temp.classList.add("todo-app__item-detail_checked");
            }
        }
}

function active_f(){
    remove_f();
    for(i = 0; i < Active_box.length; i++)
        someCodeToCreateNewItem(All_box[Active_box[i]-1], Active_box[i]);
}

function completed_f(){
    remove_f();
    for(i = 0; i < Completed_box.length; i++){
        someCodeToCreateNewItem(All_box[Completed_box[i]-1], Completed_box[i]); 
        var temp = document.getElementById(Completed_box[i]);
        temp.checked = true;
        temp = document.getElementById(Completed_box[i]+"h");
        temp.classList.add("todo-app__item-detail_checked");
    }
}

function clean_f(){
    const box = document.getElementsByClassName("todo-app__item-detail_checked");
    var id_li;
    var temp;
    var l = box.length;
    if(box.length == 0){
        for(i = 0; i < Completed_box.length; i++)
            All_box[Completed_box[i]-1] = 0;
        Completed_box = [];
    }
    else{
        for(i = 0; i < l; i ++){
            id_li = parseInt(box[0].id, 10);
            temp = document.getElementById(id_li+"li");
            list.removeChild(temp);
        }
        for(i = 0; i < Completed_box.length; i++)
            All_box[Completed_box[i]-1] = 0;
        Completed_box = [];
    }
}

function Delete(id){
    id = parseInt(id, 10);
    var index;
    var temp = document.getElementById(id+"li");
    list.removeChild(temp);
    All_box[id-1] = 0;
    index = Active_box.indexOf(id);
    if(index > -1)
        Active_box.splice(index, 1);
    else{
        index = Completed_box.indexOf(id);
        if(index > -1)
            Completed_box.splice(index, 1);}
    left.innerHTML = (Active_box.length+" Left");
    if(Active_box.length + Completed_box.length === 0){
        footer = document.getElementById("todo-footer");
        root.removeChild(footer);}
}

function Done(id){
    var index;
    const check = document.getElementById(id);
    const word_style = document.getElementById(id+"h");
    id = parseInt(id, 10); 
    if (check.checked == true){
        word_style.classList.add("todo-app__item-detail_checked");
        Completed_box.push(id);
        index = Active_box.indexOf(id);
        Active_box.splice(index, 1);}
    else{
        word_style.classList.remove("todo-app__item-detail_checked");
        Active_box.push(id);
        index = Completed_box.indexOf(id);
        Completed_box.splice(index, 1);}
    left.innerHTML = Active_box.length+" Left";
}

function someCodeToCreateNewItem(val, id){
    const li = document.createElement("LI");
    const div = document.createElement("DIV");
    const label = document.createElement("LABEL");
    const put = document.createElement("INPUT");
    const h1 = document.createElement("H1");
    const img = document.createElement("IMG");
    li.setAttribute("class", "todo-app__item");
    li.setAttribute("id", id+"li");
    div.setAttribute("class", "todo-app__checkbox");
    label.setAttribute("for", id);
    put.setAttribute("id", id);
    put.setAttribute("type", "checkbox");
    put.setAttribute("onClick", "Done(this.id)");
    h1.setAttribute("class", "todo-app__item-detail");
    h1.setAttribute("id", id+"h");
    img.setAttribute("class", "todo-app__item-x");
    img.setAttribute("id", id+"img");
    img.setAttribute("onClick", "Delete(this.id)");
    list.appendChild(li);
    li.appendChild(div);
    div.appendChild(put);
    div.appendChild(label);
    li.appendChild(h1);
    li.appendChild(img);
    img.src="./img/x.png";
    h1.innerHTML=val;
    left.innerHTML = (Active_box.length+" Left");
}

input.addEventListener('keyup', event => {
    if (event.keyCode === 13 && event.target.value !== '') {
        All_box.push(input.value);
        if(Active_box.length + Completed_box.length === 0)
            addfooter();
        t += 1;
        Active_box.push(t);
        someCodeToCreateNewItem(input.value, t);
        input.value = "";
    }
  });
