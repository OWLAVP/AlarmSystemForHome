<?php
    $period = $_GET['period'];
    $now = new DateTime();
    $unixtime_now = $now->getTimestamp();
    $unixtime_lastday = $unixtime_now - 86400;
    $unixtime_lastweek = $unixtime_now - 604800;
    $unixtime_lastmonth = $unixtime_now - 2592000;
    $unixtime_lastyear = $unixtime_now - 31104000;

    $db = new SQLite3('/mnt/usb/.data/main.db'); 
    $sql_lastday="SELECT id, id_sensor, t_data,time_date FROM temperature WHERE time_date <= $unixtime_now AND time_date >= $unixtime_lastday" ;
    $sql_lastweek="SELECT id, id_sensor, t_data,time_date FROM temperature WHERE time_date <= $unixtime_now AND time_date >= $unixtime_lastweek";
    $sql_lastmonth="SELECT id, id_sensor, t_data,time_date FROM temperature WHERE time_date <= $unixtime_now AND time_date >= $unixtime_lastmonth";   
    $sql_lastmonth="SELECT id, id_sensor, t_data,time_date FROM temperature WHERE time_date <= $unixtime_now AND time_date >= $unixtime_lastmonth";
    $sql_lastyear="SELECT id, id_sensor, t_data,time_date FROM temperature WHERE time_date <= $unixtime_now AND time_date >= $unixtime_lastyear";

    if($period == "lastday"){
        $result = $db->query($sql_lastday);
    }
    else if($period == "lastweek")
    {
        $result = $db->query($sql_lastweek);
    }
    else if($period == "lastmonth")
    {
        $result = $db->query($sql_lastmonth);
    }
    else if($period == "lastyear")
    {
        $result = $db->query($sql_lastyear);
    }    

    $array = array();
    while($data = $result->fetchArray(SQLITE3_ASSOC))
    {
         $array[] = $data;
    }
    foreach($array as $row){
        $dataset[] = array($row['id_sensor'], $row['time_date'],$row['t_data']);
    }
    $db->close();  
//write_to_console(json_encode($dataset));

echo json_encode($dataset);
 


/*function write_to_console($data) {
    $console = $data;
    if (is_array($console))
        $console = implode(',', $console);

    echo "<script>console.log('Console: " . $console . "' );</script>";
}*/

//write_to_console(json_encode($dataset));
//write_to_console([1,2,3]);
        //var_dump("sqlite load") ;

?>