
thing happened:
---------------------------------------------------------
echo $
echo "$"
echo '$'
echo -n




i think we have problem with this case:
---------------------------------------------------------
echo -n nenie_iri
echo -nn nenie_iri
echo -n -n -n nenie_iri
echo "-n" nenie_iri
echo -n"-n" nenie_iri
echo "-nnnn" nenie_iri
echo "-n -n -n"-n nenie_iri
echo "-n -n" nenie_iri
echo "-n '-n'" nenie_iri

echo '$USER'
echo "'$USER'"
echo " '$USER' "
echo nenie_iri"'$USER'" ' $USER '
echo ' $USER '
echo nenie_iri "$USER" "$USER"nenie_iri
echo $USER'$USER'text oui oui oui oui $USER oui $USER ''

echo nenie"$USER"iri
echo '$USER' "$USER" "nenie \' iri"
echo "$USER""Users/$USER/file""'$USER'"'$USER'
echo '"$USER"''$USER'"""$USER"
echo " $USER "'$PWD'
echo $?
