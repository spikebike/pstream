#!/usr/bin/perl

while (<>)
{
# >shanghai 1t<
	if (/t\>(2 x \S+ [1248]t)\<\/t/)
	{
#		print "FOOO $1\n";
		print "<text  onclick='ToggleOpacity(evt, \"$1\")'>$1</text>\n";
		$vg=$1;
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
