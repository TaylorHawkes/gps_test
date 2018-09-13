<?php

$points=file_get_contents("lat_lng.txt");
$points = preg_split('/\r\n|\r|\n/',$points);
array_pop($points);
$fpoints=[];
foreach($points as $p){
    $pe=explode(",",$p);
    $ps=new StdClass();
    $ps->lat=$pe[0];

    $ps->lng=$pe[1];
    $fpoints[]=$ps;
//  $p["lat"]=$pe[0];
//  $p["lng"]=$pe[1];
}

file_put_contents("lat_lng.txt","");
echo json_encode($fpoints);

?>

