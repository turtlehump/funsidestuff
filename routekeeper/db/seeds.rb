users = User.create(
  [
    { email: 'sa@sa.com', password: "sa", password_confirmation: "sa", admin: true},
    { email: 'a@a.com', password: "a", password_confirmation: "a"},
    { email: 'b@b.com', password: "b", password_confirmation: "b"},
    { email: 'c@c.com', password: "c", password_confirmation: "c"},
    { email: 'd@d.com', password: "d", password_confirmation: "d"},
    { email: 'e@e.com', password: "e", password_confirmation: "e"},
    { email: 'f@f.com', password: "f", password_confirmation: "f"},
    { email: 'g@g.com', password: "g", password_confirmation: "g"},
    { email: 'h@h.com', password: "h", password_confirmation: "h"},
    { email: 'i@i.com', password: "i", password_confirmation: "i"},
    { email: 'j@j.com', password: "j", password_confirmation: "j"},
    { email: 'k@k.com', password: "k", password_confirmation: "k"},
    { email: 'l@l.com', password: "l", password_confirmation: "l"},
    { email: 'm@m.com', password: "m", password_confirmation: "m"},
    { email: 'n@n.com', password: "n", password_confirmation: "n"},
    { email: 'o@o.com', password: "o", password_confirmation: "o"},
    { email: 'p@p.com', password: "p", password_confirmation: "p"},
    { email: 'q@q.com', password: "q", password_confirmation: "q"},
    { email: 'r@r.com', password: "r", password_confirmation: "r"},
    { email: 's@s.com', password: "s", password_confirmation: "s"},
    { email: 't@t.com', password: "t", password_confirmation: "t"},
    { email: 'u@u.com', password: "u", password_confirmation: "u"},
    { email: 'v@v.com', password: "v", password_confirmation: "v"},
    { email: 'w@w.com', password: "w", password_confirmation: "w"},
    { email: 'x@x.com', password: "x", password_confirmation: "x"},
    { email: 'y@y.com', password: "y", password_confirmation: "y"},
    { email: 'z@z.com', password: "z", password_confirmation: "z"},
    { email: 'phil@phil.com', password: "phil", password_confirmation: "phil"},
  ]
)

starts = Start.create(
  [
    {name: "Home", description: "only setting address", address: "16731 Magneson Loop, Los Gatos CA", user_id: 28},
    {name: "Home2", description: "only setting lat/long", latitude: 37.233878, longitude: -121.96236, user_id: 28},
    {name: "Home3", description: "mismatch between address and lat/long", address: "16731 Magneson Loop, Los Gatos CA", latitude: 50, longitude: 50, user_id: 28},
    {name: "Mitchs House", description: "only setting address", address: "102 Blossom Way, Scotts Valley CA", user_id: 28},
    {name: "Mitchs House2", description: "only setting lat/long", latitude: 37.05551, longitude: -121.99193, user_id: 28},
    {name: "Mitchs House2", description: "mismatch between address and lat/long", address: "102 Blossom Way, Scotts Valley CA", latitude: 50, longitude: 50, user_id: 28}
  ]
)


ends = End.create(
  [
    {name: "Chico Appartment", address: "1114 Nord Ave, Chico CA", user_id: 28}
  ]
)
