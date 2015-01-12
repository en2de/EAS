#!bin/sh

output='SlimEAS.h'
if [[ -f $output ]]; then
	rm $output
fi
touch $output

dirs='commands core base64 models'
for d in $dirs; do
	echo '//'$d >> $output
	find $d -name '*.h' -exec sh -c 'echo \#import \"$(basename {})\"' >> $output \;
	echo '' >> $output
done

cat $output

