Rebol [
    Title: {Bitwise Operator Rethinking}

    Description: {
        Considering the dynamism of Rebol and what its good at, its 
        metaprogramming domain has been too deferent to classic programming
        needs like bitwise operators and such.  These biases have befuddled
        new users and been challenged repeatedly:

            http://curecode.org/rebol3/ticket.rsp?id=1879

            http://curecode.org/rebol3/ticket.rsp?id=2128

        For those who need such semantics, rarely occuring in the core
        DSL/symbolic domains, it's possible to put them in an EXPR or MATH
        like sandbox...along with other things like operator precedence
        which puts multiplication before addition, which are also a sort
        of noise in the core system for implementing dialects.
    }
] 

;-- These aren't interesting enough applications of the symbols to belong
;-- in the core, and their infix nature makes them hard to use for 
;-- symbolic injection in evaluative dialect code.  In any case, they are
;-- unimaginative poor fits for what to do with the symbols in a space
;-- where they might have greater purpose, and harken to bowing to tradition
;-- in a sort of "IF/ELSE" kind of spirit instead of inventing EITHER and
;-- UNLESS...

|: does [do make error! {No infix bitwise "OR"... use prefix OR~}]
&: does [do make error! {No infix bitwise "AND"... use prefix AND~}]


;-- For the moment, reprogramming the infix operators is stalled because
;-- defining infix custom operators is not possible.  Stay tuned.
