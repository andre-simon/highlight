--[[
Sample plugin file for highlight 3.9
]]

Description="Add internal state IDs behind each token (for debugging purposes)."

-- optional parameter: syntax description
function syntaxUpdate(desc)
  

  function Decorate(token, state)
      return token .. ' ('.. state .. ')'
  end
end

Plugins={

  { Type="lang", Chunk=syntaxUpdate },

}
