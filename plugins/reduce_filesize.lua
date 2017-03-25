Description="Reduce output file size by ignoring numbers and operators"

-- optional parameter: syntax description
function syntaxUpdate(desc)
  Digits = '(?!x)x'
  Operators = None
end

Plugins={
  { Type="lang", Chunk=syntaxUpdate },
}
