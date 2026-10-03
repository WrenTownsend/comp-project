## top level stuff
<program> --> <top-elems> "eof"

<top-elems> --> {<top-elem>}

<top-elem> --> <func-def> | <top-declaration>

## function stuff
<func-def> --> <func-signature> <compound-statement>
<func-signature> --> "fn" ID "(" ")" TODO: implement <id_list>

## statments's
<compound-statement> --> "{"<statments>"}"

<statements> --> {<statement>}

## statments
<statement> --> <decl-stmt> | <if-stmt> | <while-stmt> | ...

<decl-stmt> --> "var" "id" ";"
<assign-stmt> --> "id" "=" <expr> ";"
<if-stmt> --> "if" "(" <expr> ")" <compound-statement>
<while-stmt> --> "while" "(" <expr> ")" <compound-statement>
<return-stmt> --> "return" ";"
<output-stmt> //todo
<input-stmt> //todo

## Expression Rules (an expression is a thing that returns a value)
<expr> --> <term> {("+"|"-") <term>}

<term> --> <factor> {("*"|"/") <factor>}


<factor> --> ID | INT_LIT | "("<expr>")"

## terminal

ID
INT_LIT

## test program ##
<program>
	<top-elem>
		<func-def>
			<func-signature>
				"fn"
				"id: main"
				<id-list>
			<compound-statement>
				"{"
				<statments>
					<statment>
						<decl-stmt>
							"var"
							"id"
							";"
					<statment>
					<statment>
						<assign>
							"id"
							"="
							<expr>
					<statment>
					<statment>
					<statment>
					<statment>
				"}"
