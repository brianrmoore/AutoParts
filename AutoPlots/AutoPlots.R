#!/usr/bin/Rscript

library(RColorBrewer)

AutoPlots <- function(slog.path,height=6,width=6,auto=TRUE){

	slog <- readLines(slog.path)
	elements <- getElements(slog)
	tree <- getRestaurant(slog,elements$element_order,'treelength')
	asrv <- getRestaurant(slog,elements$element_order,'asrv')
	subrates <- getRestaurant(slog,elements$element_order,'subrates')
	basefreq <- getRestaurant(slog,elements$element_order,'basefreq')

	layout.mat <- matrix(1:4,nrow=2)
	if(auto) quartz(height=height,width=width)
	layout(layout.mat)
	par(mar=c(0,3,3,1),oma=c(8,3,0,0))
	plotRestaurant(elements,asrv,x=FALSE)
	plotRestaurant(elements,subrates)
	plotRestaurant(elements,basefreq,x=FALSE,y=FALSE)
	plotRestaurant(elements,tree,y=FALSE)
	
	# layout.mat <- matrix(1:6,nrow=2)
	# layout.mat[2,3] <- 5
	# quartz(height=8*2/3,width=8)
	# layout(layout.mat)
	# par(mar=c(0,3,3,1),oma=c(3,3,0,0))
	# plotRestaurant(elements,asrv,x=FALSE)
	# plotRestaurant(elements,subrates)
	# plotRestaurant(elements,basefreq,x=FALSE,y=FALSE)
	# plotRestaurant(elements,tree,y=FALSE)
	# plotPartitions(elements)
	
}

plotPartitions = function(elements){

	fill_colors <- brewer.pal(3,'Blues')
	fill_colors[1] <- 'white'
	border_colors <- heat.colors(n=max(elements$sorted_border_colors))
	border_colors[1] <- 'black'

	y_vals <- 0
	for(i in 2:length(elements$sorted_shapes)){
		if(elements$sorted_shapes[i-1] == elements$sorted_shapes[i]){
			y_vals <- c(y_vals,y_vals[i-1]-1)
		} else {
			y_vals <- c(y_vals,y_vals[i-1]-2)
		}
	}

	plot(1:10,type='n',xaxt='n',yaxt='n',bty='n',ylim=range(y_vals)*1.5)
	# plot(1:10)
	mtext('Data Partitions',line=0.5,adj=0,cex=1.5)
	text(x=-1.5+rep(2,length(elements$sorted_shapes)),
		y = y_vals,
		labels=1:length(elements$sorted_shapes),cex=1.5,pos=4)
	points(x=rep(2,length(elements$sorted_shapes)),
		y = y_vals,
	    pch=elements$sorted_shapes+20,cex=3,lwd=1,ljoin=1,
	    bg=fill_colors[elements$sorted_colors],
	    col=border_colors[elements$sorted_border_colors])
	text(x=0.5+rep(2,length(elements$sorted_shapes)),
		y = y_vals,
		labels=gsub('_',' ',elements$sorted_element_names),cex=1.5,pos=4)
	
}

plotRestaurant = function(elements,restaurant,x=TRUE,y=TRUE){

	fill_colors <- brewer.pal(3,'Blues')
	fill_colors[1] <- 'white'
	border_colors <- heat.colors(n=max(elements$sorted_border_colors))
	border_colors[1] <- 'black'
	
	plot(x=1:length(restaurant$sorted_assignment_vector),
	     y=restaurant$sorted_assignment_vector,
	     ylab=NA,xlab=NA,yaxt='n',xaxt='n',type='n',
	     ylim=c(0.5,(0.5+max(restaurant$sorted_assignment_vector))),
	     xlim=c(0.5,length(restaurant$sorted_assignment_vector)+0.5),
	     bty='n',yaxs='i')
	for(i in seq(1,max(restaurant$sorted_assignment_vector),by=2)){
		polygon(x=c(-5,(max(elements$element_order)+5),(max(elements$element_order)+5),-5),
		        y=c(i-0.5,i-0.5,i+0.5,i+0.5),
		        border=NA,
		        col='#B4E2F450')
	}
	points(x=1:length(restaurant$sorted_assignment_vector),
	     y=restaurant$sorted_assignment_vector,
	     pch=elements$sorted_shapes+20,cex=1.5,lwd=1.5,ljoin=1,
	     bg=fill_colors[elements$sorted_colors],
	     col=border_colors[elements$sorted_border_colors])
	box()
	axis(side=2,at=1:max(restaurant$sorted_assignment_vector),las=1,cex.axis=1.5)
	if(x){
		axis(side=1,at=1:length(restaurant$sorted_assignment_vector),las=2,
		labels=gsub('_',' ',elements$sorted_element_names),cex.axis=1.5)
	} 
	if(y) mtext(expression(paste('Partition Scheme, ',sigma)),side=2,line=2.5,cex=1.2)
	legend(x=0,y=0.99*par('usr')[4],legend=restaurant$num_credible_schemes,bty='n',cex=2,adj=1)
	mtext(restaurant$name,line=0.5,adj=0,cex=1.5)

}

getRestaurant <- function(slog,order,name){
		
	start_line <- 1
	while(!grepl(paste('Restaurant:',name),slog[start_line])){
		start_line <- start_line+1
	}
	
	part_string <- strsplit(slog[start_line+1],'')[[1]]
	assignment_vector <- part_string[(1+which(part_string == '(')):(which(part_string == ')')-1)]
	sorted_assignment_vector <- as.numeric(assignment_vector[assignment_vector != ','][order])
	
	while(!grepl('Unique partitions',slog[start_line])){
		start_line <- start_line+1
	}
	num_credible_schemes <- 0
	prob <- 0
	while(TRUE){
		start_line <- start_line+1
		this_line <- slog[start_line]
		this_line <- strsplit(this_line,'--')[[1]][2]
		this_line <- strsplit(this_line,' ')[[1]]
		prob <- prob + as.numeric(this_line[!this_line %in% ''][2])
		num_credible_schemes <- num_credible_schemes+1
		if(prob > 0.95) break
	}
	new_name <- switch(name,treelength='Tree Length',basefreq='Base Frequencies',asrv='ASRV',subrates='Substitution Rates')
	res <- list(sorted_assignment_vector=sorted_assignment_vector, num_credible_schemes=num_credible_schemes,name=new_name)

	return(res)	
	
}

getElements <- function(slog){

	start_line <- 1
	while(!grepl('User-defined data partitions:',slog[start_line])){
		start_line <- start_line+1
	}
	start_line <- start_line+1
	end_line <- start_line
	while(grepl(';',slog[end_line])){
		end_line <- end_line+1
	}
	end_line <- end_line-1
	charsets <- slog[start_line:end_line]
	ends <- sapply(strsplit(charsets,'-'),function(x) strsplit(x[2],'\\\\')[[1]][1])
	unique_ends <- unique(ends)
	num_unique_ends <- length(unique(ends))
	shapes <- match(ends,unique_ends)
	colors <- unlist(lapply(1:num_unique_ends,function(x) 1:(sum(shapes == x))))
	border_colors <- rep(1,length(shapes))
	for(i in 1:length(ends)){
		border_colors[shapes > 5*i] <- border_colors[shapes > 5*i] + 1
	}

	element_order <- order(shapes)
	sorted_charsets <- charsets[element_order]
	sorted_shapes <- shapes[element_order]
	sorted_colors <- colors[element_order]
	sorted_border_colors <- border_colors[element_order]
	sorted_element_names <- sapply(sorted_charsets,function(set){
		s <- strsplit(strsplit(set,'charset')[[1]][2],'')[[1]]
		part_name <- gsub(' ','',paste(s[1:(which(s == '=')-1)],collapse=''))
		return(part_name)
	},USE.NAMES=FALSE)

	element_summary <- list(element_order=element_order,sorted_charsets=sorted_charsets,
		 sorted_shapes=sorted_shapes,sorted_element_names=sorted_element_names,
		 sorted_colors=sorted_colors,sorted_border_colors=sorted_border_colors)
	
	return(element_summary)
	
}

args <- commandArgs(TRUE)

if(length(args) > 0){
	path <- args[2]
	output <- gsub('.slog','_parts.pdf',path)
	pdf(file=output,height=6,width=6)
	AutoPlots(path,auto=FALSE)
	dev.off()	
}

