
Description="Add internal state IDs behind each token (for debugging)."

function syntaxUpdate(desc)
  function Decorate(token, state)
    return token .. ' ('.. string.format("%d",state) .. ')'
  end
end

Plugins={

  { Type="lang", Chunk=syntaxUpdate },

}
