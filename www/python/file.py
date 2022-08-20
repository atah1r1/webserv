# create full webpage with style in a multiline variable named body
body = """<html>
<head>
<title>Home</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>

<nav class='main hList inactive'>
	<div class="tab active"><span>About</span></div><!--
	--><div class="tab"><span>Work</span></div><!--
	--><div class="tab"><span>Profiles</span></div><!--
	--><div class="tab" ><span>Store</span></div><!--
	--><div class="tab" ><span>Tours</span></div><!--
	--><div class="tab" ><span>Videos</span></div><!--
	--><div class="tab" ><span>News</span></div><!--
	--><div class="tab" ><span>Underground</span></div><!--
	--><div class="tab" ><span>Connect</span></div>
	</nav>
	<page id="home">
	<span class="pagetag">#HOME</span>
	<section class="header">
		<div>
		<h2><span>W</span><span>ELCOME</span></h2>
		<h5>COLDPLAY</h5>
		</div>
		<div class='logo'></div>
	</section>
	<section id="grid_wrapper" class="init">
		<card>
		<div>
			<span>ABOUT</span>
		</div>
		</card>
		<card>
		<div>
			<span>WORK</span>
		</div>
		</card>
		<card>
		<div>
			<span>PROFILES</span>
		</div>
		</card>
		<card>
		<div>
			<span>STORE</span>
		</div>
		</card>
		<card>
		<div>
			<span>TOURS</span>
		</div>
		</card>
		<card>
		<div>
			<span>VIDEOS</span>
		</div>
		</card>
		<card>
		<div>
			<span>NEWS</span>
		</div>
		</card>
		<card>
		<div>
			<span>UNDERGROUND</span>
		</div>
		</card>
	</section>
	</page>
	<div id="content_wrapper" class='inactive'>
	<button class="close">&#xf00d;</button>
	<div class='content'>
		<section>
		<h2>#1</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#2</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#3</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#4</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#5</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#6</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#7</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
		<section>
		<h2>#8</h2>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		<p>
			Here's some random shit for this random tab on this random template.
		</p>
		</section>
	</div>
	</div>
	<div id="footer">
	<span style="display:block;font-size:0.9em;line-height:55px;"><span style="display:inline-block;">&copy; <span style="font-family:'Titillium Web';font-weight:900;letter-spacing:0.06em">CALVRIX</span> Design 2016</span></span>
	<span style="display:block;font-size:0.7em;line-height:30px;margin-top:-25px;color:rgba(255,255,255,0.15)">&lang; !--kidding obviously-- &rang;<span>
	</div>

	<script src="https://code.jquery.com/jquery-3.6.0.min.js" integrity="sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=" crossorigin="anonymous"></script>
	<script src="script.js"></script>

</body>
</html>"""

print("Status: 200 OK\r")
print("Content-Type: text/html\r")
print("Content-Length:", len(body), "\r")
print("\r")
# print(body, end="")