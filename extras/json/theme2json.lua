#!/bin/lua

-- This script converts a highlight theme to a JSON dataset

function bool2str(b)
   if not b then return "false" end
   return "true"
end

function printAttributes(e, isLast)
    retVal =  [[{ "Colour": "]]..e.Colour..[[", "Bold": ]]..bool2str(e.Bold)..[[, "Italic": ]]..bool2str(e.Italic)..[[, "Underline": ]]..bool2str(e.Underline)
    if isLast then
        retVal = retVal .." }\n"
    else
        retVal = retVal .." },\n"
    end
    return retVal
end

function printKeywords()
    retVal="\n" 
    for k, v in pairs(Keywords) do
        retVal = retVal.."        "..printAttributes(v, k==#Keywords)
     end
   return retVal
end

function theme2json()
    dofile (arg[1])
    print (
[[{
    "Description"   : "]]..Description..[[",
    "Default"       : ]]..printAttributes(Default)..[[
    "Canvas"        : ]]..printAttributes(Canvas)..[[
    "Number"        : ]]..printAttributes(Number)..[[
    "Escape"        : ]]..printAttributes(Escape)..[[
    "String"        : ]]..printAttributes(String)..[[
    "StringPreProc" : ]]..printAttributes(StringPreProc)..[[
    "BlockComment"  : ]]..printAttributes(BlockComment)..[[
    "LineComment"   : ]]..printAttributes(LineComment)..[[
    "PreProcessor"  : ]]..printAttributes(PreProcessor)..[[
    "LineNum"       : ]]..printAttributes(LineNum)..[[
    "Operator"      : ]]..printAttributes(Operator)..[[
    "Interpolation" : ]]..printAttributes(Interpolation)..[[
    "Keywords": []]..printKeywords()..[[
    ]
}]])
end

if #arg < 1 then
   print ("Invoke this script with a theme file as argument")
else
    if not pcall(theme2json) then
        print ("Script not existing or invalid")
    end
end

