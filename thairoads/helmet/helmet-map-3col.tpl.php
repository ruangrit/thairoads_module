<div id="maps">
  <div class="map-header">
    <div class="img"><img src="/sites/all/themes/theme472/images/logo.png" /></div>
    <div class="date"><?php print date('d/m/Y');?></div>
  </div>
  <div class="map-title"><h2><?php print drupal_get_title();?></h2></div>
  <div class="maps-description">

    <div>อัตราการสวมหมวกนริภัยรายจังหวัด<?php print $term_name; ?></div>

    <div>ผลจากการสํารวจในภาคสนามด้วยวิธีการสังเกต (Observational Survey) ครอบคลุมพื้น ที่ใน 77 จังหวัด ทั่วว ทั้ง ประเทศ </div>

    <div>แหล่งข้อมูล</div>

    <div>
      มูลนิธิไทยโรดส์และเครือข่ายเฝ้าระวังสถานการณ์ความปลอดภัยทางถนน (Road Safety Watch)
      
    </div>

    
  </div>
  <div style="display:none" id="warning_term_level" class='warning'>
  กำหนดขอบเขตไม่ครบ หรือ ยังไม่ได้กำหนดขอบเขตการแสดงสีของหมวดหมู่นี้
  </div>
  <div id="edit_term_level">
  <?php 
  if (user_access('administer taxonomy')) {
    print l('ตั้งค่าขอบเขตการแสดงสีของหมวดหมู่นี้', 'admin/content/taxonomy/edit/term/'.$cate_id, array('query' => array('destination' => 'statistic/watch/detail/'.$sub_cate_id)));
  }
  ?>
  </div>

  <div class="main-filter">
    <div class="filter-year">
      
        <div class="filter-g1">
          <input type="radio" checked name="filter1_choice" value="year" id="filter1year">
          <label for="filter1year"> ข้อมูลปี</label>
          <select id="filter1" class="filter-year-choice">
          </select>  
        </div> 

    </div>
    <div class="filter-cat"></div>
  </div>

  <div class="main-map">
  <?php foreach($code_detail['id'] as $key => $value):?>

    <div class="map map-<?php print $key;?>">
      <div class="print">
        <input type="button" value="<?php print t('print'); ?>" class="print-btn">
      </div>
      <div class="filter">
        <input type="hidden" class="code" value="<?php print $value;?>">
        <input type="hidden" class="cateId" value="<?php print $cate_id;?>">
      </div>
      <div class="map-description"><?php print $code_detail['name'][$key];?></div>

      <div id="svgmap<?php print $key;?>">Loading...</div>
      <div class="top-ten">
        
      </div>

      <div class="maps-info-province">
          <div id="svgmap<?php print $key;?>-info" class="info hide">
            <h3>ข้อมูลแต่ละจังหวัด</h3>
            <?php foreach($province_list as $k => $province):?>
              <?php if ($k == 0) { print '<div class="province-group group-first">'; } ?>
              <?php if ($k == 38) { print '<div class="province-group group-second">'; } ?>
              <div class="maps-info-item">
                <div class="maps-info-name"><?php print $province->name; ?></div>
                <div class="maps-info-value <?php print $province->tid;?>"></div>
              </div>
              <?php if ($k == 37 || $k == (count($province_list) - 1)) { print '</div>'; } ?>
            <?php endforeach;?>
          </div>
      </div>


    </div>

  <?php endforeach;?>
    
  </div>

  <div class="map-info-color">
    <?php $run_num = 1; $map_unit = '%';?>
    <?php foreach($term_list as $key => $value):?>
      <?php if($run_num == 1):?>
        <div class="color-left">
      <?php endif;?>
      <?php if($run_num == 4):?>
        <div class="color-right">
      <?php endif;?>
    <div class="color-item">
      <div class="color">
        <img src="/sites/all/modules/thairoads/myhook/img/<?php print $value['img'];?>" />
      </div>
      <div class="level">
      <?php if($run_num != count($term_list) && $value['start'] != ''):?>
        <?php print thairoads_num_format($value['start'], 3).' - '.thairoads_num_format($value['end'], 3).' '.$map_unit; ?>
      <?php elseif($value['start'] != ''):?>
        <?php print "> ".thairoads_num_format($value['start'], 3).' '.$map_unit;?>
      <?php else:?>
        ยังไม่ได้กำหนดค่า
      <?php endif;?>
      </div>
    </div>
      <?php if($run_num == 3 || $run_num == 6):?>
        </div>
      <?php endif;?>
    <?php $run_num++;?>
    <?php endforeach;?>
  </div>
  
  <div class="map-url-wrapper" >
    <div class="map-url">URL: <?php print  "http://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]"; ?></div>
    <div class="map-sitename">มูลนิธิไทยโรดส์</div>
  </div>

</div>
