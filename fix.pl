#!/usr/bin/perl

while (<>)
{
	if (/\>(.*)-thread (.*)\</)
	{
		$vg=$1.$2;
#		print "$1 $2 vg=$vg\n";
		print "<text  onclick='ToggleOpacity(evt, \"$vg\")'>$1-thread $2</text>\n";
	}
	elsif ((/\<path d=/) && ($vg))
	{	
		s/path d/path onclick='ToggleOpacity(evt, "$vg")' id='$vg' d/;
		print $_;
	}
	elsif (/www.w3.org\/1999\/xlink/)
	{
		print (" xmlns:xlink=\"http://www.w3.org/1999/xlink\" onload='Init(evt)'>\n");
	}
	elsif (/<defs>/)
	{
		print "<defs>\n";
		open($hide,"<hide.svg");
		while (<$hide> )
		{
			print $_;
		}
	}
	else
	{
		print $_;
	}

}
