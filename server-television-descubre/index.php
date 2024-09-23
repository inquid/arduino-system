<?php
error_reporting(-1);

if($_GET['c'])
{
    if($_GET['c']==1){
        system("omxplayer videos/VideoEstudio.mp4");
	echo "done";
    }
    if($_GET['c']==2){
	system("omxplayer audios/AudioSalirTerraza.wav");
	echo "done";
    }
   }else{
        echo "nothing recived";
}
