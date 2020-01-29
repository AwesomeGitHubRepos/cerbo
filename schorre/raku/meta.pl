 sub isalpha($x) { return $x (elem) (['a'..'z'] (|) ['A'..'Z']) ; }
 sub isdigit($x) { return $x (elem) ['0'..'9']; }

 say isalpha('q');
 say isalpha('0');
