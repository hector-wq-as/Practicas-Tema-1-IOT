function cambiarTemperatura(){
    let temperatura=document.getElementById("temp");
    temperatura.innerText = (20+(Math.random()*10)).toFixed(2);
    
}
setInterval(cambiarTemperatura, 2000);