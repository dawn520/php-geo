<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('geo')) {
	dl('geo.' . PHP_SHLIB_SUFFIX);
}
$module = 'geo';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";
#var_dump(confirm_geo_compiled("hello world!"));
#var_dump(geo_get_distance(22.5485102977, 114.0610885620, 22.5486688364, 114.0867948532));
var_dump(geo_get_distance(22.5485102977, 114.0610885620, 22.5486688364, 114.0867948532));
?>
