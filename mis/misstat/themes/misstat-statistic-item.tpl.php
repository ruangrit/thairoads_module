<div class="spi-item <?php print $class ?>">
    <?php if ($image): ?>
    <div class="spi-image"><img src="<?php print $image; ?>" /></div>
    <?php endif; ?>
  <div class="spi-topic">
    <div class="spi-content">
      <h5 class="title"><?php print $title ?></h5>
      <p><?php print $description ?></p>
    </div>
    <div class="spi-more arrow-right">more</div>
  </div>
  <?php if ($items) :?>
  <ul class="spi-list<?php if ($expanded) print ' expanded'; ?>">
    <?php foreach ($items as $spi): ?>
    <li class="<?php print $spi['class'] ?>">
      <?php print $spi['title'] ?> 
      <?php if ($spi['link']) print $spi['link'] ?>
    </li>
    <?php endforeach; ?>
  </ul>
  <?php endif; ?>
</div>
