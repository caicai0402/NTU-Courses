const who = "Cai Cai";
const columnIndex = ["Subject", "Score"];
const scoreCard = {
    name: `${who}`,
    records: [
        ["Chinese", 98],
        ["English", 99],
        ["Math", 100],
    ],
};

class Row{
    constructor(data, tag){
        this.tr = document.createElement("tr");
        for(let i = 0; i < data.length; i++){
            let td = document.createElement(tag);
            td.textContent = data[i];
            this.tr.appendChild(td)
        }
    }
    get node(){return this.tr;}
}

const table = document.getElementById("table");

const caption = document.createElement("caption");
caption.textContent = `${scoreCard.name}'s Score`;
table.appendChild(caption);

const thead = document.createElement("thead");
thead.appendChild(new Row(columnIndex, "th").node);
table.appendChild(thead);

const tbody = document.createElement("tbody");
table.appendChild(tbody);
for(let i = 0; i < scoreCard.records.length; i++){
    tbody.appendChild(new Row(scoreCard.records[i], "td").node);
}
