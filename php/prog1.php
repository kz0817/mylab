<?php
$name = '"james"';
$var = "<a>$name</a>\n";
print $var ;

$foo = 'foo"<script>hoge</script> "';
echo <<<EOT
My name is "$foo".
EOT;
?>
