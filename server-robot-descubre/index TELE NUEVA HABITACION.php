<?php
error_reporting(-1);

//print_r($jsondoc);
if($_GET['c'])
{
    if($_GET['c']==1){
        system("omxplayer videos/VIDEO-HABITACION.mp4");
    }
    if($_GET['c']==2){
	system("omxplayer audios/10AUDIO-DESPEDIDA.wav");
    }
}else{
        echo "nothing recived";
}
