<?php

function rstr($n = 32)
{
  $a = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
  $r = "";
  $c = strlen($a) - 1;
  for ($i = 0; $i < $n; $i++) {
    $r .= $a[rand(0, $c)];
  }
  return $r;
}

$n = 100000;
$r = [];
for ($i = 0; $i < $n; $i++) {
  $r[] = rstr(100);
}

ob_start();
echo "#include \"hashtable.h\"\n\n";
echo "void\nht_construct(HashTable *ht)\n{\n";
foreach ($r as $k => $rand) {
  echo "  insert_strstr_ht(ht, \"{$rand}\", \"Data {$k}\");\n";
}
echo "}\n";
file_put_contents("ht_insert.c", ob_get_clean());


ob_start();
echo "const char list[{$n}][2][200] = {\n";
foreach ($r as $k => $rand) {
  echo "{\"{$rand}\", \"Data {$k}\"},\n";
}
echo "};";
file_put_contents("no_hash_list.c", ob_get_clean());
