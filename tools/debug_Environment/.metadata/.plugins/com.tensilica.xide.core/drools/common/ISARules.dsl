[consequence][]Report error=String[] fields = new String[]{"\{option1\}","\{option2\}","\{value\}"}; addAutoGenMessage(messageSet, controller, msgKey, fields, drools.getRule().getName(), MCMessage.ERROR, fields);
[consequence][]Report custom error:"{errormsg}"=String[] fields = new String[]{"\{option1\}","\{option2\}"}; addMessage(messageSet, controller, drools.getRule().getName(), "{errormsg}", MCMessage.ERROR, fields);
[consequence][]Report custom error: "{errormsg}",\[{commaseparatedfields}\]=String[] fields = new String[]{\{commaseparatedfields\}}; addMessage(messageSet, controller, drools.getRule().getName(), "{errormsg}", MCMessage.ERROR, fields);
[consequence][]Report custom warning: "{msg}",\[{commaseparatedfields}\]=String[] fields = new String[]{\{commaseparatedfields\}}; addMessage(messageSet, controller, drools.getRule().getName(), "{msg}", MCMessage.WARNING, fields);
[consequence][]Report warning=String[] fields = new String[]{"\{option1\}","\{option2\}","\{value\}"}; addAutoGenMessage(messageSet, controller, msgKey, fields, drools.getRule().getName(), MCMessage.WARNING, fields);
[consequence][]consoleprint : "{msg}"=System.out.println("{msg}");

[condition][]'{option1}' requires '{option2}'=(ConfigOptions({option1} == 1 && {option2} != 1) && MessageKeys(msgKey:=SimpleDepBool))
[condition][]'{option1}' conflicts with '{option2}'=(ConfigOptions( {option1} == 1 && {option2} != 0 ) && MessageKeys(msgKey:=SimpleDepBoolNot))

[condition][]'{option1}' requires that '{option2}' is         "{value}"=(ConfigOptions( {option1} == 1 && {option2} != "{value}" ) && MessageKeys(msgKey:=DepBoolString))
[condition][]'{option1}' requires that '{option2}' is equal to {value}=(ConfigOptions( {option1} == 1 && {option2} != {value}) && MessageKeys(msgKey:=DepBoolString))
[condition][]'{option1}' requires that '{option2}' is at least {value}=(ConfigOptions( {option1} == 1 && {option2} < {value}) && MessageKeys(msgKey:=DepBoolNumber))
[condition][]'{option1}' equals "{value1}" requires that '{option2}' is not "{value}"=(ConfigOptions( {option1} == "{value1}" && {option2} == "{value}" ) && MessageKeys(msgKey:=DepBoolStringNot))
[condition][]'{option1}' equals "{value1}" requires that '{option2}' is not equal to {value}=(ConfigOptions( {option1} == "{value1}" && {option2} == {value}) && MessageKeys(msgKey:=DepBoolStringNot))
[condition][]'{option1}' requires that '{option2}' cannot exceed {value}=(ConfigOptions( {option1} == 1 && {option2} > {value}) && MessageKeys(msgKey:=DepBoolMaxNumber))
[condition][]OR=and
[condition][]AND=or
[condition][]println "{msg}"=eval(println("{msg}"))
[condition][]printvar '{variable}'=ConfigOptions(option:{variable}) eval(println(""+option))

[condition][]'{option1}' equals "{value1}" requires that '{option2}' is "{value}"=(ConfigOptions( {option1} == "{value1}" && {option2} != "{value}" ) && MessageKeys(msgKey:=DepBoolString))
[condition][]'{option1}' notequals "{value1}" requires that '{option2}' is "{value}"=(ConfigOptions( {option1} != "{value1}" && {option2} != "{value}" ) && MessageKeys(msgKey:=DepBoolString))

[condition][]'{option1}' equals "{value1}" requires that '{option2}' is equal to {value}=(ConfigOptions( {option1} == "{value1}" && {option2} != {value}) && MessageKeys(msgKey:=DepBoolString))
[condition][]'{option1}' equals "{value1}" requires that '{option2}' is at least {value}=(ConfigOptions( {option1} == "{value1}" && {option2} < {value}) && MessageKeys(msgKey:=DepBoolNumber))

[condition][]'{option1}' equals "{value1}" requires that '{option2}' cannot exceed {value}=(ConfigOptions( {option1} == "{value1}" && {option2} > {value}) && MessageKeys(msgKey:=DepBoolMaxNumber))
[condition][]'{option1}' equals "{value1}" requires that '{option2}' one of "{value2}"=(ConfigOptions( {option1} == "{value1}"  && {option2} matches "{value2}") && MessageKeys(msgKey:=SimpleDepBool))
[condition][]'{option1}' equals "{value1}" requires that '{option2}' is between {value2} and {value3}=(ConfigOptions( {option1} == "{value1}" && ({option2} < {value2} || {option2} > {value3})) && MessageKeys(msgKey:=DepBoolNumber))

[condition][]'{option1}' eqls "{value1}" conflicts with '{option2}'=(ConfigOptions( {option1} == "{value1}" && {option2} != 0 ) && MessageKeys(msgKey:=SimpleDepBoolNot))

[condition][]'{option1}' requires that '{option2}' is not "{value}"=(ConfigOptions( {option1} == 1 && {option2} == "{value}" ) && MessageKeys(msgKey:=DepBoolStringNot))
[condition][]'{option1}' requires that '{option2}' is not equal to {value}=(ConfigOptions( {option1} == 1 && {option2} == {value}) && MessageKeys(msgKey:=DepBoolStringNot))

[condition][]'{option1}' requires that '{option2}' is one of {value1} or {value2}=(ConfigOptions( {option1}==1 && !({option2} == {value1} || {option2} == {value2})) && MessageKeys(msgKey:=DepBoolNumber))

# These are all multi-part.
# Use "Report custom error:"; do not trust auto

# A and (B or C) requires D eq value
[condition][]'{option1}' and either of '{option2}' or '{option3}' requires that '{option4}' equals {value1}=(ConfigOptions( {option1} == 1 && ({option2} == 1 || {option3} == 1) && {option4} != {value1} ) && MessageKeys(msgKey:=DepBoolNumber))
# A and (B == v1) requires C == v2
[condition][]'{option1}' together with '{option2}' equal to {value1} require that '{option3}' equals {value2}=(ConfigOptions( {option1} == 1 && {option2} == {value1} && {option3} != {value2} ) && MessageKeys(msgKey:=DepBoolNumber))

# A>0 requires B=value
[condition][]'{option1}' greater than {value1} requires that '{option2}' eq {value2}=(ConfigOptions( {option1} > {value1} && {option2} != {value2} ) && MessageKeys(msgKey:=DepBoolNumber))

# A with B>0 requires C=value or C>value
[condition][]'{option1}' together with '{option2}' greater than {value1} requires that '{option3}' equals {value2}=(ConfigOptions( {option1} == 1 && {option2} > {value1} && {option3} != {value2} ) && MessageKeys(msgKey:=DepBoolNumber))
[condition][]'{option1}' together with '{option2}' equals {value1} requires that '{option3}' equals {value2}=(ConfigOptions( {option1} == 1 && {option2} == {value1} && {option3} != {value2} ) && MessageKeys(msgKey:=DepBoolNumber))
[condition][]'{option1}' together with '{option2}' equals {value1} requires that '{option3}' gt {value2}=(ConfigOptions( {option1} == 1 && {option2} == {value1} && {option3} <= {value2} ) && MessageKeys(msgKey:=DepBoolNumber))
[condition][]'{option1}' together with '{option2}' greater than {value1} requires symmetry of '{option3}'=(ConfigOptions( {option1} == 1 && {option2} > {value1} && {option2} != {option3} ) && MessageKeys(msgKey:=DepBoolNumber))
[condition][]'{option1}' together with '{option2}' greater than {value1} requires that '{option2}' is ge {value2}=(ConfigOptions( {option1} == 1 && {option2} > {value1} && {option2} < {value2} ) && MessageKeys(msgKey:=DepBoolNumber))

[condition][]'{option1}' together with '{option2}' equals {value1} needs feature "{feat}"=(ConfigOptions( {option1} == 1 && {option2} == {value1} && !hasFeature(controller, "{feat}") ) && MessageKeys(msgKey:=DepBoolNumber))

[condition][]'{option1}' as "{value1}" plus '{option2}' gt {value2} necessitates '{option3}'=(ConfigOptions( {option1} == "{value1}" && {option2} > {value2} && {option3} == 0 ) && MessageKeys(msgKey:=DepBoolNumber) )


# A == "B" requires either C or D
[condition][]'{option1}' equals "{value1}" requires either '{option2}' or '{option3}'=(ConfigOptions( {option1} == "{value1}" && (! ({option2} == 1 || {option3} == 1) ) ) && MessageKeys(msgKey:=DepBoolNumber))
# A == "B" and C == "D" requires / recommends {E | either E or F}
[condition][]'{option1}' eq "{value1}" plus '{option2}' eq "{value2}" requires either '{option3}' or '{option4}'=(ConfigOptions( {option1} == "{value1}" && {option2} == "{value2}" && (! ({option3} == 1 || {option4} == 1) ) ) && MessageKeys(msgKey:=DepBoolNumber))
[condition][]'{option1}' equ "{value1}" plus '{option2}' equ "{value2}" requires '{option3}'=(ConfigOptions( {option1} == "{value1}" && {option2} == "{value2}" && (!({option3} == 1) ) ) && MessageKeys(msgKey:=DepBoolNumber))

[condition][]'{option1}' equals "{value1}" requires '{option2}'=(ConfigOptions( {option1} == "{value1}" && {option2} != 1) && MessageKeys(msgKey:=SimpleDepBool))
[condition][]'{option1}' equals "{value1}" requires '{option2}'=(ConfigOptions( {option1} == "{value1}" && {option2} != 1 ) && MessageKeys(msgKey:=SimpleDepBoolNot))

[condition][]'{option1}' requires symmetry of '{option2}' and '{option3}' and '{option4}'=(ConfigOptions( {option1} == 1 && (!({option2} == {option3} && {option3} == {option4}))) && MessageKeys(msgKey:=SimpleDepBool))

# standalone individual requirements on options
[condition][]'{option1}' requires minimum ISA "{minISA}"=(ConfigOptions( {option1} == 1 && eval(!hasMinISA(controller, "{minISA}"))))
[condition][]'{option1}' requires feature "{feature}"=(ConfigOptions( {option1} == 1 && eval(!hasFeature(controller, "{feature}"))))
[condition][]'{option1}' nonzero requires feature "{feature}"=(ConfigOptions( {option1} != "0" && eval(!hasFeature(controller, "{feature}"))))
[condition][]'{option1}' requires family "{family}"=(ConfigOptions( {option1} == 1 && eval(!isSameFamily(controller, "{family}"))))
[condition][]'{option1}' requires minimum XPG {version}=(ConfigOptions( {option1} == 1 && eval(!hasMinXPG(controller,configBuildOptions, {version}))))
[condition][]'{option1}' requires minimum Xplorer version {version}=(ConfigOptions( {option1} == 1 && eval(!hasMinXplorer(controller,{version}))))

# expressions evaluate better in functions. drools parser is seriously fragile / unpredictable
[condition][]'{option1}' nequ "{value1}" req minx {xv} orfeat "{feat}"=(ConfigOptions( {option1} != "{value1}" && !hasMinXXorFeature(controller,{xv},"{feat}")))
[condition][]'{option1}' notequal "{value1}" withisa "{isav}" req minxx {xv} orfeat "{feat}"=(ConfigOptions( {option1} != "{value1}" && !hasISAreqMinXXorFeature(controller,"{isav}",{xv},"{feat}") ) )
[condition][]'{option1}' notequal "{value1}" withisa "{isav}" reqfeat "{feat}"=(ConfigOptions( {option1} != "{value1}" && !hasISAreqMinXXorFeature(controller,"{isav}",99999,"{feat}") ) )

# release specific ones. need to use with "Report custom error"
[condition][]'{option1}' equals "{value1}" is not supported=(ConfigOptions( {option1} == "{value1}" ))

[keyword][]condition:=when
[keyword][]failure:=then
