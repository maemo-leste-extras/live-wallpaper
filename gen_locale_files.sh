#!/bin/bash

ALL_LANGUAGES="ru_RU fi_FI es_ES es_MX pl_PL it_IT nl_NL"

xgettext --from-code=UTF-8 --keyword=_ --keyword=Q_ -f po/POTFILES.in -s -C -o po/livewp.pot
cd po
for language in $ALL_LANGUAGES; do
    echo -n "$language: "
    msgmerge -U $language.po livewp.pot
done
cd ..

exit 0
