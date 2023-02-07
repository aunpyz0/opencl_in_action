const printOffset = (offset, size) => {
	console.log("=============")
	console.log(`offset as float: ${offset}`)
	console.log(`local as float4: ${offset} ${offset+size} ${offset+size*2} ${offset+size*3}`)
	const fOffset = offset * 4
	const f4Size = size * 4
	const l1 = fOffset
	const l2 = fOffset+f4Size
	const l3 = fOffset+f4Size*2
	const l4 = fOffset+f4Size*3
	console.log(`local as float: ${l1} ${l2} ${l3} ${l4}`)
	console.log(`local 1: [${Math.floor(l1/64)}][${l1-64*Math.floor(l1/64)}]`)
	console.log(`local 2: [${Math.floor(l2/64)}][${l2-64*Math.floor(l2/64)}]`)
	console.log(`local 3: [${Math.floor(l3/64)}][${l3-64*Math.floor(l3/64)}]`)
	console.log(`local 4: [${Math.floor(l4/64)}][${l4-64*Math.floor(l4/64)}]`)
	console.log("=============")
}

const dim = (col) => {
	console.log(`dimension 64x64 ${64*64}`)
	console.log(`dimension 64x64 as float4 ${64/4*64/4}`)
	console.log(`given ${col}`)
	let size = 16
	let row = 0
	while (col >= size) {
		col -= size--
		row++
	}
	col += row
	size += row
	console.log(`col: ${col} row: ${row} size: ${size}`)
	printOffset(row * size * 4 + col, size)
	if (row != col) {
		console.log('OFF DIAGONAL')
		printOffset(col * size * 4 + row, size)
	}
}

//dim(process.argv[2])
for (let i = 0; i < process.argv[2]; ++i) {
	dim(i)
	console.log('-------------')
}
